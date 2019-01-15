
#pragma once

#include "stdint.h"

typedef uint8_t * kernel_stack_t;
typedef uint8_t * user_stack_t;

typedef struct __process_st process_t;

typedef struct context {
	void* sp;
	void* ra;
	void* s0;
	void* s1;
	void* s2;
	void* s3;
	void* s4;
	void* s5;
	void* s6;
	void* s7;
	void* s8;
	void* s9;
	void* s10;
	void* s11;
	void* satp;
	void* sepc;
} context_t;

#include "process.h"

/*
 * Save the current context on the stack and restore a previously saved context
 * previous		: data structure where the current context will be saved
 * next			: data structure where the context of the next process is saved
 */
void ctx_sw(context_t *previous, context_t *next);

/*
 * Initialize the context of a new process
 * c		: context to initialize
 * runf		: process function pointer
 * arg		: runf parameters
 */
void context_kernel_init(
		context_t *c,
		kernel_stack_t stack,
		int64_t (*runf) (void *),
		void *arg);

/*
 * Initialize the context of a new user process
 * c		: context to initialize
 * user stack pointer
 * size of user stack
 * runf		: process function pointer
 * arg		: runf parameters
 */
void context_user_init(
		context_t *c,
		user_stack_t ustack, int ssize,
		void *arg);


