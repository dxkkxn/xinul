#include "supervisor.h"

#include "csr.h"
#include "trap.h"

#include "encoding.h"
#include "stdio.h"

extern void strap_entry();


void delegate_traps()
{
	// Delegate all the supervisor interruption to the supervisor
	uint64_t interrupts = MIP_SSIP | MIP_STIP | MIP_SEIP;
	// Delegate most of the exceptions to the supervisor
	uint64_t exceptions =
			(1U << CAUSE_MISALIGNED_FETCH) |
			(1U << CAUSE_BREAKPOINT) |
			(1U << CAUSE_ILLEGAL_INSTRUCTION) |
			(1U << CAUSE_FETCH_PAGE_FAULT) |
			(1U << CAUSE_LOAD_PAGE_FAULT) |
			(1U << CAUSE_STORE_PAGE_FAULT) |
			(1U << CAUSE_USER_ECALL);

	csr_write(mideleg, interrupts);
	csr_write(medeleg, exceptions);
}

void enter_supervisor_mode()
{

	// Disable paging (bare memory)
	csr_write(satp, 0);

	// Set the trap vector (direct mode)
	csr_write(stvec, (unsigned long) strap_entry | 0UL);
	// Enable timer interrupt at a machine level to be able to catch it in
	csr_set(mie, MIP_MTIP);

	// todo : bug ici :
	// si csrw mstatus/mpp avant mie,
	// csrw mie modifi mstatus mpp à 0
	// on neva en mode user plutôt quand mode supervisor
	// set the previous context in mstatus

	csr_set(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);
	uint64_t status = csr_read(mstatus);
	printf("status :%lx\n", status);

	__asm__ __volatile__ (
	"la t0, 1f\n"
	"csrw mepc, t0\n"
	"mret\n"
	"1:":: : "t0"
	);

}
