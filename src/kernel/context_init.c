
#include <stdio.h>
#include "riscv.h"

#include "context.h"
#include "vmm.h"
#include "crt_process.h"
#include "scheduler.h"

void context_init(context_t *c, void *arg)
{
	c->ra = (void *) crt_process;
	c->s0 = sched_exit;
	c->s1 = arg;
	/* sstatus :
	 * SUM désactivé
	 * SPP : retour en mode user par défaut
	 */
	c->sstatus = (void*) SSTATUS_SUM;
}

void context_kernel_init(
		context_t *c, kernel_stack_t stack, int64_t (*runf)(void *), void *arg)
{
	context_init(c, arg);

	// Pour les programme kernel, même directory
	c->satp = get_kernel_satp().reg;

	// On demande de rester en mode supervisor lors du sret de départ
	c->sstatus = (void *) ((uint64_t) c->sstatus | SSTATUS_SPP);

	c->sepc = runf;
	c->sp = stack + K_STACK_SIZE;
	// Pas de pile user, sscratch à zéro.
	c->sscratch = 0;
}

void context_user_init(
		context_t *c,
		user_stack_t user_stack, int ssize,
		kernel_stack_t kernel_stack,
		void *arg)
{
	context_init(c, arg);

	c->sepc = (void *) PROCESS_CODE;
	c->sp = user_stack + ssize;
	c->sscratch = kernel_stack + K_STACK_SIZE;
}
