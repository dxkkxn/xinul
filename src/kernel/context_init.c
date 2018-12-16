#include "context.h"

#include "virtual_memory.h"
#include "crt_process.h"

#include <stdio.h>

#if 0
static void kernel_run(int (*runf) (void *), void arg, unsigned int magic)
{
	assert(magic == CONTEXT_MAGIC_NUMBER);

	int retval = runf(arg);
	sched_exit(retval);

	assert(0 && "kernel run exit :(");
}
#endif


void context_kernelinit(
		context_t *c, kernel_stack_t stack, int64_t (*runf) (void *), void *arg)
{
	c->satp = get_kernel_satp().reg;
	c->ra = (void*) crt_process;
	c->s0 = sched_exit;
	c->s1 = arg;
	c->sepc = runf;
	c->sp = (void *) &stack[K_STACK_SIZE-1];
}
