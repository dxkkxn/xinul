#include "scheduler.h"

#include "stdio.h"
#include "csr.h"
#include "machine.h"



/* ====      Scheduler data strucutres        ==== */

/* idle process */
static process_t kernel_idle = {
	.pid			= 0,
	.name			= "kernel_idle",
	.prio			= 0,
	.parent			= NULL,
	.status			= ACTIVE
};

static process_t* active;				/* Executing process */
static link processes[NB_SCHED_STATES]; /* scheduler's processes queue */
static link dustbin;					/* processes to be deallocated */
static context_t dummy;



/* ====      Macros use to simplify the code      ==== */
#define STATUS_QUEUE_ADD(ptr, new_status)									   \
({																			   \
	ptr->current_queue = &processes[new_status];							   \
	ptr->status = new_status;												   \
	queue_add(																   \
			ptr, &processes[new_status], process_t, status_link, prio		   \
	);																		   \
})

#define STATUS_QUEUE_DELETE(ptr)                                        \
({																		\
                ptr->current_queue = NULL;                              \
                queue_del(ptr, status_link);                            \
})

/* Process family handling */
#define FAMILY_QUEUE_ADD(ptr, parent)									\
	queue_add(ptr, &parent->children, process_t, family_link, prio)
#define FAMILY_QUEUE_DELETE(ptr)										\
	queue_del(ptr, family_link)

/* ==== Scheduler initialization ==== */

void schedule(void)
{
	printf("scheduling... ");

	process_t* old = active;
	process_t* new = queue_top(
		  &processes[ACTIVABLE], process_t, status_link
	);


	context_t* old_ctx;
	context_t* new_ctx;

	if (new == NULL) {
		assert(old != NULL && "No more processes available!");
		return;
	}

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
	old_ctx = (old != NULL)? &old->context : &dummy;
	new_ctx = &active->context;

	if (old) printf("%s [%d]", old->name, old->pid);
	else printf("null proc");
	printf(" -> ");
	if (active) printf("%s [%d]", active->name, active->pid);
	else printf("null proc");
	printf("\n");

	ctx_sw(old_ctx, new_ctx);

	// Destroy the marked processes
	while ((old = queue_out(&dustbin, process_t, status_link))) {
		process_destroy(old->pid);
	}
}

/* idle process */
int idle(void *arg)
{
	(void) arg;
	
	while (1) {
		wfi();
	}

	return 0;
}

void sched_init(){
	//init_process(); //initialiser les processus et IDLE
	//process_t kernel_idle = get_process(0); //IDLE processus

	/* Initialiser les têtes de listes */
	int i;
	for (i = 0; i < NB_SCHED_STATES; ++i) {
		INIT_LIST_HEAD(&processes[i]);
	}
	INIT_LIST_HEAD(&kernel_idle.children);
	INIT_LIST_HEAD(&dustbin);

	//TODO: restaurer le kernelinit
	context_kernelinit(
			&kernel_idle.context, kernel_idle.kernel_stack, idle, NULL
	);

	/* Set the first processus */
	STATUS_QUEUE_ADD((&kernel_idle), ACTIVABLE);
}



/* ==== Scheduling (FIFO with priorities) ==== */
static inline void sched_cleanup_children(process_t* p);
static void sched_zombify(process_t* p);

/*
 * End the current process with its returned value
 * If its init, reboot
 */
void sched_exit(int retval)
{
	process_t* p = active;
	active = NULL;
	p->info = retval;
	p->error = 0;

	if (p->pid == 1) {
		printf("TODO: reboot if process 1 dies\n");
		// TODO
		/*
		assert(0 && "reboot dans scheduler.c");
		//reboot();
		printf("It is now safe to shutdown");
		wfi();
		*/
	}

	sched_zombify(p);
	schedule();

	assert(0 && "End of function sched_exit :(");
}

/* 
 * Iterate of the process children and destroy the zombies, mark others as
 * orphans
 */
static inline void sched_cleanup_children(process_t* p)
{
	process_t* child;
	while ((child = queue_out(&p->children, process_t, family_link))) {
		if (child->status == ZOMBIE) {
			STATUS_QUEUE_DELETE(child);
			process_destroy(child->pid);
		} else {
			child->parent = NULL;
		}
	}
}

/* Kill the process of make it a zomby if it has a father */
static void sched_zombify(process_t* p)
{
        process_t* parent = p->parent;

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

#if 0
/* Fonction qui nettoye les processus finis*/
void exit_scheduling()
{
	printf("exit scheduler -> donne la main\n");
	process_t* p = active;
	active = NULL;
	//p->info = retval;
	p->error = 0;
	sched_zombify(p);
	schedule();
	assert(0 && "Retour de sched_exit()");
}
#endif

int sched_kstart(int (*run) (void *),
                 int prio,
                 const char *name,
                 void *arg)
{
	int ret_pid;

	if (prio < 1 || prio > MAXPRIO_KERNEL) {
		return -1;
	}

	// Process initialization
	process_t* p = process_create(name, 0, prio, active);
	if (p == NULL && run == NULL) {
		return -1;
	}

	// Process context initialization
	context_kernelinit(&p->context, p->kernel_stack, run, arg);

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

int sched_unblock(process_t* p)
{
	assert(p->status >= BLOCKED_ON_WSON && p->status < NB_STATES);

	STATUS_QUEUE_DELETE(p);
	STATUS_QUEUE_ADD(p, ACTIVABLE);

	return active != NULL && p->prio > active->prio;
}

/* ==== Utilitarian functions ==== */

/* Return the pid of the active process */
int sched_get_active_pid(void) 
{
        return active->pid;
}

/*
 * Réordonne la file de priorité des processus actifs 
 * après changement de la priorité du processus p
 * passé en paramètre
 */
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


/*
 * Retourne une chaîne décrivant un status
 */
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

