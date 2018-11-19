/** See LICENSE for license details.
* process.h
*
*  Created on 15 novembre 2018
*/

#pragma once

#include <stdint.h>

// Structure d'un process.
typedef struct desc_proc * process_t;
typedef uint64_t * kernel_stack_t;
typedef uint64_t * user_stack_t;

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
	RUN,
	READY,
	SLEEP,
	WAIT_PID,
	ZOMBIE,
	DEAD,
	SYSTEM,
	WAIT_MSG,
	WAIT_READ,
	WAIT_SEM,
	WAIT_INTR
};

struct cpu_state {
	uint64_t SP;
};

struct desc_proc {
	int pid;
	int priority;
	int zero; // utilisé comme prio pour prendre la queu pour une fifo (toujours à zéro
	process_t parent;
	//list_process_t childrens;
	char *name;
	enum state_e state;
	int return_value;
	//link linker;
	uint32_t wakeup;
	int waitting_for;
	int children_return_value;
	struct cpu_state cpu_state;
	uint32_t user_stack_size;
	kernel_stack_t kernel_stack;
};

// process API

void init_process();
process_t create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg);
int getpid();

/**
void ps();

int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
int start_in_user(const char *process_name, unsigned long ssize, int prio, void *arg);
int start_in_kernel(int (*pt_func)(void*), const char *name, int prio, void *arg);
process_t creer_process_user(const char *code_name, const char *nom, int priority, int stack_size, void *arg);


int waitpid(int pid, int *retvalp);
void exit(int retval);
int kill(int pid);
void kill_current();

char *mon_nom(void);
uint16_t mon_pid(void);
size_t get_nbproc();

int getpid();
int getprio(int pid);
int chprio(int pid, int newprio);
process_t getproc(int pid);
process_t get_process_actif();
*/

