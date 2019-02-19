#include "supervisor_trap.h"
#include "trap.h"

#include "stdio.h"
#include "encoding.h"
#include "csr.h"
#include "machine.h"
#include "machine_trap.h"
#include "scheduler.h"
#include "kbd.h"
#include "timer.h"
#include "sbi.h"

typedef void (*trap_handler)(struct trap_frame *);

void dump_tf(struct trap_frame *tf)
{
	printf("\nSupervisor trap exception\n");
	printf("sp 0x%lx", tf->sp);
	printf("\tgp 0x%lx", tf->gp);

	printf("\nra 0x%lx", tf->ra);

	printf("\na0 0x%lx", tf->a0);
	printf("\ta1 0x%lx", tf->a1);
	printf("\ta2 0x%lx", tf->a2);
	printf("\ta3 0x%lx", tf->a3);

	printf("\na4 0x%lx", tf->a4);
	printf("\ta5 0x%lx", tf->a5);
	printf("\ta6 0x%lx", tf->a6);
	printf("\ta7 0x%lx", tf->a7);

	printf("\npc 0x%lx\nstval 0x%lx\ninstruction 0x%x\nsstatus 0x%lx\n",
		   tf->sepc, tf->stval,
		   (uint32_t) tf->insn, tf->sstatus);
}


static void handle_instruction_access_fault(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Instruction access fault!");
}

static void handle_load_access_fault(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Load access fault!");
}

static void handle_store_access_fault(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Store/AMO access fault!");
}

static void handle_illegal_instruction(struct trap_frame *tf)
{
	tf->insn = *(uint32_t *) tf->sepc;
	dump_tf(tf);
	die("An illegal instruction was executed!");
}

static void handle_breakpoint(struct trap_frame *tf)
{
	dump_tf(tf);
	printf("Breakpoint!\n");
	tf->sepc += 4;
}

static void handle_misaligned_fetch(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Misaligned instruction access!");
}

static void handle_misaligned_load(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Misaligned Load!");
}

static void handle_misaligned_store(struct trap_frame *tf)
{
	dump_tf(tf);
	die("Misaligned AMO!");
}

static void segfault(struct trap_frame *tf, void *addr, const char *type)
{
	dump_tf(tf);
	const char *who = (tf->sstatus & SSTATUS_SPP) ? "Kernel" : "User";
	die("%s %s segfault @ 0x%p", who, type, addr);
}

static void handle_fault_fetch(struct trap_frame *tf)
{
//	if (handle_page_fault(tf->badvaddr, PROT_EXEC) != 0)
	segfault(tf, (void *) tf->stval, "fetch");
}

static void handle_fault_load(struct trap_frame *tf)
{
//	if (handle_page_fault(tf->badvaddr, PROT_READ) != 0)
	segfault(tf, (void *) tf->stval, "load");
}

static void handle_fault_store(struct trap_frame *tf)
{
//	if (handle_page_fault(tf->badvaddr, PROT_WRITE) != 0)
	segfault(tf, (void *) tf->stval, "store");
}


void strap_handler(struct trap_frame *tf)
{
	const static trap_handler strap_handlers[] = {
			[CAUSE_MISALIGNED_FETCH] = handle_misaligned_fetch,
			[CAUSE_FETCH_ACCESS] = handle_instruction_access_fault,
			[CAUSE_ILLEGAL_INSTRUCTION] = handle_load_access_fault,
			[CAUSE_STORE_ACCESS] = handle_store_access_fault,
			[CAUSE_FETCH_PAGE_FAULT] = handle_fault_fetch,
			[CAUSE_ILLEGAL_INSTRUCTION] = handle_illegal_instruction,
			[CAUSE_BREAKPOINT] = handle_breakpoint,
			[CAUSE_MISALIGNED_LOAD] = handle_misaligned_load,
			[CAUSE_MISALIGNED_STORE] = handle_misaligned_store,
			[CAUSE_LOAD_PAGE_FAULT] = handle_fault_load,
			[CAUSE_STORE_PAGE_FAULT] = handle_fault_store,
	};

	if (tf->scause & INTERRUPT_CAUSE_FLAG) {
		switch (tf->scause & ~INTERRUPT_CAUSE_FLAG) {
			case intr_s_timer:
				// Set a new timer interrupt
				clock_handler();
				break;
			case intr_s_external:
				keyboard_handler();
				csr_clear(sip, MIP_SEIP);
				break;
			default:
				die(
						"supervisor mode: unhandable interrupt %ld @ %p",
						tf->scause & ~INTERRUPT_CAUSE_FLAG, (void *) tf->sepc
				);
		}
	} else {
		assert(tf->scause < ARRAY_SIZE(strap_handlers) && strap_handlers[tf->scause]);

		strap_handlers[tf->scause](tf);


//				die(
//						"supervisor mode: unhandable exception %ld @ %p",
//						tf->scause, (void *) tf->sepc
//				);
	}
}
