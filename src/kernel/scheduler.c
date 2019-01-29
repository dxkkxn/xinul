#include "scheduler.h"

#include "stdio.h"
#include "stdlib.h"
#include "csr.h"
#include "machine.h"

#include "supervisor_trap.h"
#include "vmm.h"
#include "program.h"
#include "crt_process.h"
#include "mem.h"


/* ====      Scheduler data strucutres        ==== */

/* idle process */
static process_t kernel_idle = {
		.pid            = 0,
		.name            = "kernel_idle",
		.prio            = 0,
		.parent            = NULL,
		.status            = ACTIVE
};

static process_t *active;                /* Executing process */
static link processes[NB_SCHED_STATES]; /* scheduler's processes queue */
static link dustbin;                    /* processes to be deallocated */
static context_t dummy;

/* ====      Macros use to simplify the code      ==== */
#define STATUS_QUEUE_ADD(ptr, new_status)                                       \
({                                                                               \
    ptr->current_queue = &processes[new_status];                               \
    ptr->status = new_status;                                                   \
    queue_add(                                                                   \
            ptr, &processes[new_status], process_t, status_link, prio           \
    );                                                                           \
})

#define STATUS_QUEUE_DELETE(ptr)                                        \
({                                                                        \
                ptr->current_queue = NULL;                              \
                queue_del(ptr, status_link);                            \
})

/* Process family handling */
#define FAMILY_QUEUE_ADD(ptr, parent)                                    \
    queue_add(ptr, &parent->children, process_t, family_link, prio)

#define FAMILY_QUEUE_DELETE(ptr)                                        \
    queue_del(ptr, family_link)

/* ====      Internal functions      ==== */

/* 
 * Iterate of the process children and destroy the zombies, mark others as
 * orphans
 */
static inline void sched_cleanup_children(process_t *p);

/* Kill the process of make it a zomby if it has a father */
static void sched_zombify(process_t *p);

static inline void sched_cleanup_children(process_t *p)
{
	process_t *child;
	while ((child = queue_out(&p->children, process_t, family_link))) {
		if (child->status == ZOMBIE) {
			STATUS_QUEUE_DELETE(child);
			process_destroy(child->pid);
		} else {
			child->parent = NULL;
		}
	}
}

static void sched_zombify(process_t *p)
{
	process_t *parent = p->parent;

	sched_cleanup_children(p); //TODO implanter cette fonction

	if (parent == NULL) {
		/* Mark the process */
		queue_add(p, &dustbin, process_t, status_link, prio);
		return;
	}

	/* Become a zomby and unblock the father */
	STATUS_QUEUE_ADD(p, ZOMBIE);
	if (parent->status == BLOCKED_ON_WSON &&
		(parent->info < 0 || p->pid == parent->info)) {
		sched_unblock(parent);
	}
}

/*
 * Wait of son process
 * pid		: son PID or -1
 * retvalp	: pointer on the return value of the child or NULL
 * nohang	: null if blocking function
 */
static inline int sched_waitpid_full(int pid, int64_t *retvalp, int nohang)
{
	process_t *child;
	int ret;

	/* Si le processus n'existe pas */
	if (pid == 0 || pid > NBPROC)
		return -1;

	/* Récupérer le fils et vérifier qui est le pere ! */
	child = (pid > 0)
			? process_get(pid)
			: queue_top(&active->children, process_t, family_link);

	if (child == NULL || child->parent != active)
		return -1;

	/* Si le processus n'est pas zombie il faut l'attendre */
	if (child->status != ZOMBIE) {
		if (nohang)
			return -1;
		sched_block(pid, &processes[BLOCKED_ON_WSON],
					BLOCKED_ON_WSON, NULL);
	}

	ret = child->pid;
	//ret = (child->error)? 0 : child->pid;

	if (retvalp != NULL)
		*retvalp = child->info;

	STATUS_QUEUE_DELETE(child);
	FAMILY_QUEUE_DELETE(child);
	process_destroy(child->pid);

	return ret;
}



/* ====      Scheduler initialization      ==== */

/* idle process */
int64_t idle(void *arg)
{
	(void) arg;

	while (1) {
		ENABLE_SUPERVISOR_INTERRUPTS();
		wfi();
	}

	return 0;
}

void sched_init()
{
	// Initialize the heads of the lists
	int i;
	for (i = 0; i < NB_SCHED_STATES; ++i) {
		INIT_LIST_HEAD(&processes[i]);
	}
	INIT_LIST_HEAD(&kernel_idle.children);
	INIT_LIST_HEAD(&dustbin);

	// Set idle's context
	kernel_idle.kernel_stack = mem_alloc(K_STACK_SIZE);
	context_kernel_init(
			&kernel_idle.context, kernel_idle.kernel_stack, idle, NULL
	);

	// Set the first process
	STATUS_QUEUE_ADD((&kernel_idle), ACTIVABLE);
}

int sched_kstart(int64_t (*run)(void *),
				 int prio,
				 const char *name,
				 void *arg)
{
	int ret_pid;

	if (prio < 1 || prio > MAXPRIO_KERNEL) {
		return -1;
	}

	// Process initialization
	process_t *p = process_create(name, prio, active);
	if (p == NULL && run == NULL) {
		return -1;
	}

	// Process context initialization
	context_kernel_init(&p->context, p->kernel_stack, run, arg);

	// PID to return (watch out: schedule())
	ret_pid = p->pid;

	STATUS_QUEUE_ADD(p, ACTIVABLE);
	if (active != NULL) {
		FAMILY_QUEUE_ADD(p, active);
	}
	if (active == NULL || prio > active->prio) {
		schedule();
	}

	return ret_pid;
}

int sched_ustart(const char *name,
				 unsigned long ssize,
				 int prio,
				 void *arg)
{
	if (prio < 1 || prio > MAXPRIO) {
		return -1;
	}

	if (ssize < 0
		|| ssize > STACK_SIZE_MAX) {
		return NULL;
	}
	if (ssize < 100) {
		ssize = 100;
	}

	// User process initialization
	process_t *p = process_user_create(name, prio, active, ssize);
	if (p == NULL) {
		return -1;
	}

	// User process context initialization
	context_user_init(&p->context, p->user_stack, ssize, p->kernel_stack, arg);

	STATUS_QUEUE_ADD(p, ACTIVABLE);
	if (active != NULL) {
		FAMILY_QUEUE_ADD(p, active);
	}
	if (active == NULL || prio > active->prio) {
		schedule();
	}

	return p->pid;
}


/* ====      Scheduling (FIFO with priorities)      ==== */
void schedule(void)
{
	process_t *old = active;
	process_t *new = queue_top(
			&processes[ACTIVABLE], process_t, status_link
	);


	context_t *old_ctx;
	context_t *new_ctx;

	if (new == NULL) {
		assert(old != NULL && "No more processes available!");
		return;
	}
	int8_t pid = -2;
	if (old) pid = old->pid;
	else pid = -1;
printf("schedul %d -> %d\n", pid, new->pid);
	// set the new active process
	if (old == NULL || old->status != ACTIVE || old->prio <= new->prio) {
		STATUS_QUEUE_DELETE(new);
		active = new;

		// if old process is still activable put it on the corresponding queue
		if (old != NULL && old->status == ACTIVE) {
			STATUS_QUEUE_ADD(old, ACTIVABLE);
		}
	}

	active->status = ACTIVE;
	old_ctx = (old != NULL) ? &old->context : &dummy;
	new_ctx = &active->context;
/*
	printf("\nscheduling... ");
	if (old) printf("%s [%d]", old->name, old->pid);
	else printf("null proc");
	printf(" -> ");
	if (active) printf("%s [%d]", active->name, active->pid);
	else printf("null proc");
	printf("\n");
*/
	ctx_sw(old_ctx, new_ctx);

	// Destroy the marked processes
	while ((old = queue_out(&dustbin, process_t, status_link))) {
		process_destroy(old->pid);
	}
}

void sched_exit(int retval)
{
	process_t *p = active;
	active = NULL;
	p->info = retval;
	p->error = 0;

	if (p->pid == 1) {
		printf("End of process with pid=1, OSON halt.\n");
		exit(0);
	}

	sched_zombify(p);
	schedule();

	assert(0 && "End of function sched_exit :(");
}

int sched_kill(int pid)
{
	process_t *p = process_get(pid);

	if (p == NULL) {
		return -1;
	}

	// Can't kill a zombie
	if (p->status == ZOMBIE) {
		return -1;
	}

	p->info = 0;
	p->error = pid;

	if (p != active) {
		STATUS_QUEUE_DELETE(p);
		sched_zombify(p);
		return 0;
	}

	// self destruction
	active = NULL;
	sched_zombify(p);
	schedule();

	assert(0 && "Return after kamikaze sched_kill");
	return -1;
}

int sched_get_active_pid(void)
{
	return active->pid;
}

int sched_get_active_prio(void)
{
	return active->prio;
}

int sched_block(long info, link *queue, int new_status, int *error)
{
	int retval;

	assert(active != NULL
		   && new_status < NB_STATES
		   && new_status >= BLOCKED_ON_WSON);

	active->current_queue = queue;
	active->status = new_status;
	active->info = info;
	active->error = 0;

	if (new_status != BLOCKED_ON_CLOCK) {
		queue_add(active, queue, process_t, status_link, prio);
	} else {
		queue_add(active, queue, process_t, status_link, info);
	}

	schedule();

	if (error != NULL) {
		*error = active->error;
	}
	retval = active->info;
	active->info = 0;
	active->error = 0;
	return retval;
}

int sched_unblock(process_t *p)
{
	assert(p->status >= BLOCKED_ON_WSON && p->status < NB_STATES);

	STATUS_QUEUE_DELETE(p);
	STATUS_QUEUE_ADD(p, ACTIVABLE);

	return active != NULL && p->prio > active->prio;
}

int sched_waitpid(int pid, int64_t *retvalp)
{
	return sched_waitpid_full(pid, retvalp, 0);
}

int sched_waitpid_nohand(int pid, int64_t *retvalp)
{
	process_t *p;

	if (pid >= 0) {
		return sched_waitpid_full(pid, retvalp, 1);
	}

	queue_for_each(p, &active->children, process_t, family_link)
	{
		if (p->status == ZOMBIE) {
			return sched_waitpid_full(p->pid, retvalp, 1);
		}
	}
	return -1;
}

int sched_chprio(int pid, int newprio)
{
	int oldprio;
	process_t *top;
	process_t *p = process_get(pid);

	if (p == NULL || newprio < 1 || newprio > MAXPRIO)
		return -1;

	if (p->status == ZOMBIE)
		return -1;

	oldprio = p->prio;
	p->prio = newprio;

	if (p->status == BLOCKED_ON_CLOCK)
		return oldprio;

	if (p->parent != NULL) {
		FAMILY_QUEUE_DELETE(p);
		FAMILY_QUEUE_ADD(p, (p->parent));
	}

	if (p->status != ACTIVE) {
		/* Si p n'est pas le processus actif
		 * il faut modifier l'ordre de sa file de priorité */
		assert(p->current_queue != NULL);
		queue_del(p, status_link);
		queue_add(p, p->current_queue, process_t, status_link, prio);
	}

	top = queue_top(&processes[ACTIVABLE], process_t, status_link);
	assert(active != NULL);
	if (top != NULL && active->prio < top->prio)
		schedule();

	return oldprio;
}


int sched_printstatus(process_t *p, char *buffer, unsigned int size)
{
	switch (p->status) {
		case ACTIVE:
			return snprintf(buffer, size, "active");
		case ACTIVABLE:
			return snprintf(buffer, size, "activable");
		case ZOMBIE:
			return snprintf(buffer, size, "zombie");
		case BLOCKED_ON_CLOCK:
			return snprintf(buffer, size, "sleeping");
		case BLOCKED_ON_WSON:
			return snprintf(buffer, size, "waiting %d", (int) p->info);
		case BLOCKED_ON_MSG:
			return snprintf(buffer, size, "blocked on message queue");
		case BLOCKED_ON_IO:
			return snprintf(buffer, size, "blocked on I/O");
		default:
			return snprintf(buffer, size, "unknown");
	}
}

