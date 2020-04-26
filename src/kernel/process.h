/** See LICENSE for license details.
* process.h
*
*  Created on 15 novembre 2018
*/

#pragma once

#include "stdint.h"

#include "context.h"
#include "queue.h"
#include "vmm/pmm.h"


typedef struct __process_st process_t;

#include "scheduler.h"
#include "vmm/vmm.h"


typedef struct list_head list_process_t;
extern process_t process_alive;
// Nombre max de process
#define NBPROC 100
#define MAX_LENGTH_PROCESS_NAME 20
// Taille de la stack kernel pour chaque process
#define K_STACK_SIZE 2048
// Taille max de la pile user
#define STACK_SIZE_MAX 100000
#define MAXPRIO 256
#define MAXPRIO_KERNEL 260
//Taille du Heap User
#define HEAP_USER_SIZE 4096


struct __process_st {
	// process information
	int pid;
	char *name;
	int prio;

	// Process "family"
	process_t *parent;
	link children;

	// Execution context
	context_t context;
	struct pmm_varea *code_varea;
	uint32_t user_stack_size;
	user_stack_t user_stack;
	struct pmm_varea *user_stack_varea;
	struct pmm_varea *user_heap_varea;
	kernel_stack_t kernel_stack;

	// Virtual memory
	pagetable_t page_dir;
	struct shm_handle *shm_handle;

	// Scheduler part
	long info;
	int error;
	int status;
	link *current_queue;
	link status_link;
	link family_link;
};

#if 0
// process API

void init_process();
#endif

/* Allocate and initialize a process structure */
process_t *process_create(
		const char *name,
		int prio,
		process_t *parent);

/* Allocate and initialize a user process structure */
process_t *process_user_create(
		const char *name,
		int prio,
		process_t *parent,
		int ssize);

/* Destroy the "pid" process */
int process_destroy(int pid);

#if 0
process_t* create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg);
process_t* create_user_process(const char *code_name, const char *nom, int priority, int stack_size, void *arg);
#endif

/* Return the priority of the process pid if it exists, else -1 */
int process_getprio(int pid);

/* Return the processus matching with the given pid */
process_t *process_get(int pid);
