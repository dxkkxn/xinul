#include "supervisor.h"

#include "csr.h"
#include "machine_trap.h"
#include "encoding.h"
#include "stdio.h"

extern void strap_entry();


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

	// Disable paging (bare memory)
	csr_write(satp, 0);

	// set the previous context in mstatus

	// Set the trap vector (direct mode)
	csr_write(stvec, (unsigned long)strap_entry | 0UL);
	// Enable timer interrupt at a machine level to be able to catch it in
	csr_set(mie, MIP_MTIP);
	csr_set(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);
	uint64_t status = csr_read(mstatus);
	printf("status :%lx\n",status);

	__asm__ __volatile__ (
			"la t0, 1f\n"
			"csrw mepc, t0\n"
			"mret\n"
			"1:" ::: "t0"
	);

}
