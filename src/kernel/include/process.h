/** See LICENSE for license details.
* process.h
*
*  Created on 15 novembre 2018
*/

#pragma once

#include "stdint.h"
#include "context.h"
#include "queue.h"

// Structure d'un process.
typedef struct desc_proc * process_t;
typedef uint64_t * kernel_stack_t;
typedef uint64_t * user_stack_t;
typedef struct list_head list_process_t;
extern process_t process_alive;
// Nombre max de process
#define NBPROC 100
#define MAX_LENGTH_process_NAME 20
// Taille de la stack kernel pour chaque process
#define STACK_SIZE 1024
// Taille max de la pile user
#define STACK_SIZE_MAX 100000
#define MAXPRIO 256
#define MAXPRIO_KERNEL 260
//Taille du Heap User
#define HEAP_USER_SIZE 4096

enum state_e {
	/* Processus en cours d'exécution */
	ACTIVE = -1,

	/* États gérés par l'ordonnanceur */
	ACTIVABLE = 0,
	ZOMBIE,
	BLOCKED_ON_WSON,
	NB_SCHED_STATES,

	/* Autres états*/
	BLOCKED_ON_MSG = NB_SCHED_STATES,
	BLOCKED_ON_IO,
	BLOCKED_ON_CLOCK,
	NB_STATES
};

struct desc_proc {
	int pid;
	int prio;
	int zero; // utilisé comme prio pour prendre la queu pour une fifo (toujours à zéro
	process_t parent;
	link children;
	char *name;
	enum state_e state;
	int return_value;
	//link linker;
	uint32_t wakeup;
	int waitting_for;
	int children_return_value;
	struct context context;
	uint32_t user_stack_size;
	kernel_stack_t kernel_stack;
	//scheduler variables
	long info;
  int error;
	link *current_queue;
  link status_link;
  link family_link;
};

// process API

void init_process();
process_t create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg);
process_t create_user_process(const char *code_name, const char *nom, int priority, int stack_size, void *arg);
int getpid();
process_t get_process(int pid);
