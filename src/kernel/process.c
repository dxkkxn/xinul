/** See LICENSE for license details.
* process.c
*
*  Created on 15 novembre 2018
*/

#include "process.h"

#include "string.h"
#include "stdio.h"
#include "mem.h"

#include "vmm.h"
#include "pmm.h"
#include "userspace_apps.h"
#include "context.h"
#include "crt_process.h"
#include "csr.h"


process_t *processes[NBPROC];


static void process_copy_user_code(process_t *p, const struct uapps *app)
{
	assert(p != NULL);
	assert(app != NULL);

	// Switch to process directory to copy code at 1GiB
	satp_csr parent_satp = {.ureg = csr_read(satp)};
	csr_write(satp, p->context.satp);

	/* Copy the code */
	memcpy((uint64_t *) PROCESS_CODE,
		   (uint64_t *) app->start,
		   (uint64_t) app->end - (uint64_t) app->start);

	// Remise en place du directory appelant.
	csr_write(satp, parent_satp.ureg);
}

int process_create_code_space(process_t *p)
{
	unsigned int code_size;
	const struct uapps *app;
	assert(p != NULL);

	/* Evaluation de la taille du code du programme */
	app = find_app(p->name);
	if (app == NULL) {
		return -1;
	}
	code_size = (uint64_t) app->end - (uint64_t) app->start;
	assert(code_size > 0);

	/* Réservation de l'espace mémoire */
	p->code_varea = pmm_create_ucode(p, (void *) PROCESS_CODE, (code_size & 0xFFFFF000u) + 0x1000);
	assert(p->code_varea != NULL);

	/* Copie du code */
	process_copy_user_code(p, app);

	return 0;
}

void process_create_user_heap(process_t *p)
{
	p->user_heap_varea = pmm_create_udata(p, (void *) PROCESS_HEAP, HEAP_USER_SIZE);
}

void process_create_user_stack(process_t *p, int size)
{
	unsigned local_size;
	assert(p != NULL);
	assert(size != 0);

	local_size = (size & 0xFFFFF000) + 0x1000;
	p->user_stack_varea = pmm_create_ustack(p, (void *) PROCESS_USTACK, local_size);
	assert(p->user_stack_varea != NULL);

	p->user_stack_size = local_size;
	p->user_stack = (uint8_t *) PROCESS_USTACK;
}

static inline int process_newpid(void)
{
	int i;
	for (i = 0; i < NBPROC && processes[i] != NULL; i++);
	if (i == NBPROC) {
		return -1;
	} else {
		return i + 1;
	}
}

static inline int pid_exists(int pid)
{
	return pid > 0 && pid <= NBPROC && processes[pid - 1] != NULL;
}

/* Allocate and initialize a process structure */
process_t *process_create(
		const char *name,
		int prio,
		process_t *parent)
{
	process_t *p;
	int pid;
	uint8_t name_length;

	// Find the the pid
	pid = process_newpid();
	if (pid < 1) {
		return NULL;
	}

	// allocate the process structure
	p = mem_alloc(sizeof(*p));
	if (p == NULL) {
		return NULL;
	}
	memset(p, 0, sizeof(*p));

	// copy the name of the process in the structure
	name_length = strlen(name);
	if (name_length > MAX_LENGTH_PROCESS_NAME) {
		name_length = MAX_LENGTH_PROCESS_NAME;
	}
	p->name = mem_alloc(name_length + 1);
	if (p->name == NULL) {
		return NULL;
	}
	strncpy(p->name, name, name_length);
	p->name[name_length] = 0;

	// Kernel stack allocation
	p->kernel_stack = mem_alloc(K_STACK_SIZE);
	if (p->kernel_stack == NULL) {
		return NULL;
	}

	// Structure informations
	p->pid = pid;
	p->prio = prio;
	p->parent = parent;
	INIT_LIST_HEAD(&p->children);
	p->info = 0;
	p->error = 0;

	/* Création de l'espace mémoire */
	pmm_create_basic_directory(p);

	// register the created process in the table and return its pointer
	processes[pid - 1] = p;
	return p;
}

/* Allocate and initialize a user process structure */
process_t *process_user_create(
		const char *name,
		int prio,
		process_t *parent,
		int ssize)
{
	process_t *p = process_create(name, prio, parent);
	if (p == NULL) return NULL;

	process_create_user_heap(p);

	// Stack user
	process_create_user_stack(p, ssize);

	// Allocation et copie du code du programme
	if (process_create_code_space(p) != 0) {
		printf("Program %s not found\n", name);
		// todo libéré le processus
		return NULL;
	}

	return p;
}

/* Destroy the "pid" process */
int process_destroy(int pid)
{
	process_t *p;
	if (pid < 1 || pid > NBPROC) {
		return -1;
	}
	p = processes[pid - 1];
	if (p == NULL)
		return -1;

	/* Destroy shared memory mappings */
	// todo destroy memory share
//	if (p->shm_handle) {
//		shm_cleanup(p, p->shm_handle->hash);
//	}

	// Destroy user stack if needed
	if (p->user_stack_varea != NULL) {
		pmm_destroy_area(p->user_stack_varea);
		p->user_stack_varea = NULL;
		p->user_stack = NULL;
	}

	// Destroy user heap if needed
	if (p->user_heap_varea != NULL) {
		pmm_destroy_area(p->user_heap_varea);
		p->user_heap_varea = NULL;
	}

	// Destroy code space
	if (p->code_varea != NULL) {
		pmm_destroy_area(p->code_varea);
		p->code_varea = NULL;
	}

/* Free virtual memory structures */
	pmm_destroy_directory(p);

	mem_free(p->kernel_stack, K_STACK_SIZE);
	mem_free(p->name, strlen(p->name) + 1);
	mem_free(p, sizeof(*p));

	processes[pid - 1] = NULL;
	return 0;
}

#if 0
/**
 * Créer un nouveau process à partir de son code et de son nom,
 *
 * retourne NULL si erreur.
 */
process_t* create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg)
{
	if (priority < 1 || priority > MAXPRIO_KERNEL) {
		return NULL;
	}

	process_t* new = create_generic_process(name, priority);
	if (new == NULL) return NULL;
	new->context.satp = get_kernel_satp().reg;
	new->context.ra = (void*) crt_process;
	new->context.s0 = process_exit;
	new->context.s1 = arg;
	new->context.sepc = code;

	return new;
}


process_t* create_user_process(const char *code_name, const char *nom, int priority, int stack_size, void *arg)
{
	
}
#endif

/* Return the priority of the process pid if it exists, else -1 */
int process_getprio(int pid)
{
	if (!pid_exists(pid)) {
		return -1;
	}

	// We cannot get the priority of a zombie process
	if (processes[pid - 1]->status == ZOMBIE) {
		return -1;
	}

	return processes[pid - 1]->prio;
}

/* Return the processus matching with the given pid */
process_t *process_get(int pid)
{
	if (pid > 0 && pid <= NBPROC) {
		return processes[pid - 1];
	} else {
		return NULL;
	}
}
