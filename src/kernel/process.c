/** See LICENSE for license details.
* process.c
*
*  Created on 15 novembre 2018
*/

#include "process.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "virtual_memory.h"
#include "program.h"
#include "context.h"
#include "crt_process.h"


process_t* processes[NBPROC];

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
	return pid > 0 && pid <= NBPROC && processes[pid-1] != NULL;
}

/* Allocate and initialize a process structure */
process_t* process_create(
		const char *name,
		unsigned long ssize,
		int prio,
		process_t *parent)
{
	process_t* p;
	int pid;
	uint8_t name_length;

	// Find the the pid
	pid = process_newpid();
	if (pid < 1) {
		return NULL;
	}

	// allocate the process structure
	p = malloc(sizeof(*p));
	if (p == NULL) {
		return NULL;
	}
	memset(p, 0, sizeof(*p));

	// copy the name of the process in the structure
	name_length = strlen(name);
	if (name_length > MAX_LENGTH_PROCESS_NAME) {
		name_length = MAX_LENGTH_PROCESS_NAME;
	}
	p->name = malloc(name_length + 1);
	if (p->name == NULL) {
		return NULL;
	}
	strncpy(p->name, name, name_length);
	p->name[name_length] = 0;

	// Kernel stack allocation
	p->kernel_stack = malloc(K_STACK_SIZE);
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

	// register the created process in the table and return its pointer
	processes[pid-1] = p;
	return p;
}

/* Destroy the "pid" process */
int process_destroy(int pid)
{
	process_t* p;
	if (pid < 1 || pid > NBPROC) {
		return -1;
	}
	p = processes[pid-1];

	free(p->name);
	free(p->kernel_stack);
	free(p);

	processes[pid-1] = NULL;

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
	if (processes[pid-1]->status == ZOMBIE) {
		return -1;
	}

	return processes[pid-1]->prio;
}

/* Return the processus matching with the given pid */
process_t* process_get(int pid)
{
	if (pid > 0 && pid <= NBPROC) {
		return processes[pid-1];
	} else {
		return NULL;
	}
}
