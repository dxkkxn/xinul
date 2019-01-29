
#include <stdio.h>

#include "context.h"
#include "vmm.h"
#include "crt_process.h"
#include "scheduler.h"

void context_kernel_init(
		context_t *c, kernel_stack_t stack, int64_t (*runf)(void *), void *arg)
{
	c->satp = get_kernel_satp().reg;
	c->ra = (void *) crt_process;
	c->s0 = sched_exit;
	c->s1 = arg;
	c->sepc = runf;
	c->sp = stack + K_STACK_SIZE - 1;
}

void context_user_init(
		context_t *c,
		user_stack_t user_stack, int ssize,
		kernel_stack_t kernel_stack,
		void *arg)
{
	c->ra = (void *) crt_user_process;
	c->s0 = sched_exit;
	c->s1 = arg;
	c->s2 = kernel_stack + K_STACK_SIZE;
	c->sepc = (void*) PROCESS_CODE;
	c->sp = user_stack + ssize - 1;
}
