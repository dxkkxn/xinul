#include "supervisor.h"

#include "csr.h"
#include "trap.h"
#include "machine.h"
#include "encoding.h"

void extern strap_entry();

int set_next_timer_event()
{
	uint64_t delta = 100; // 1s
	// set the new mtimecmp value and activate the machine timer interruptions
	sbi_call_set_timer(delta);
}

void strap_handler(uintptr_t* regs, uintptr_t scause, uintptr_t sepc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
		case intr_s_timer:
			// Set a new timer interrupt
			set_next_timer_event();
			// Clear the interrupt flag so that the processor does not take
			// this trap again after return from interrupt
			csr_clear(sip, MIP_STIP);
			break;
		default:
			die("supervisor mode: unhandable interrupt %d @ %p", scause, sepc);
			break;
		}
	} else {
		die("supervisor mode: unhandable exception %d @ %p", scause, sepc);
	}
}

void delegate_traps()
{
		// Delegate all the supervisor interruption to the supervisor
		uint64_t interrupts = MIP_SSIP | MIP_STIP | MIP_SEIP;
		// Delegate most of the exceptions to the supervisor
		uint64_t exceptions =
			(1U << cause_instruction_address_misaligned)	|
			(1U << cause_breakpoint)						|
			(1U << cause_illegal_instruction)				|
			(1U << cause_instruction_page_fault)			|
			(1U << cause_load_page_fault)					|
			(1U << cause_store_page_fault)					|
			(1U << cause_user_ecall);
		
		csr_write(mideleg, interrupts);
		csr_write(medeleg, exceptions);
}

void enter_supervisor_mode()
{
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t)1U << 53)-1;

	// allow access to all of the memory for everyone
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// Disable paging (bare memory)
	csr_write(satp, 0);

	// set the previous context in mstatus
	csr_set(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);

	// Set the trap vector (direct mode) and enable interrupts for the
	// supervisor mode
	csr_write(stvec, (unsigned long)strap_entry | 0UL);
	csr_set(mstatus, MSTATUS_SIE);
	// Enable timer interrupt
	csr_set(mie, MIP_STIP);

	__asm__ __volatile__ (
			"la t0, 1f\n"
			"csrw mepc, t0\n"
			"mret\n"
			"1:" ::: "t0"
	);
}
