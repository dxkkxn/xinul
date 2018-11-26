// See LICENSE for license details.

#include "arch/riscv/encoding.h"
#include "machine.h"
#include "csr.h"
#include "irq.h"
#include "trap.h"

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */

#define PMP_NAPOT 0x18
#define PMP_R 0x1
#define PMP_W 0x2
#define PMP_X 0x4
#define PMP_A 0x18
#define PMP_L 0x80
#define PMP_SHIFT 0x2
#define PMP_TOR 0x8
#define PMP_NA4 0x10

int main(int argc, char **argv);

void initialize_mstatus()
{
	// potentially enable FPU, potentially enable user use of counters

	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// Enable software interrupt
	csr_write(mie, MIX_MSI);

	// Disable paging (bare memory)
	csr_write(satp, 0);
}

void delegate_traps()
{
		// Delegate all the supervisor interruption to the supervisor
		uint64_t interrupts = MIX_SSI | MIX_STI | MIX_SEI;
		// Delegate most of the exceptions to the supervisor
		uint64_t exceptions =
			(1U << cause_instruction_address_misaligned)	|
			(1U << cause_breakpoint)						|
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
	csr_set(mstatus, MSTATUS_MPP_MASK_S);
	csr_clear(mstatus, MSTATUS_MPIE_MASK);
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	__asm__ __volatile__ (
			"la t0, 1f\n"
			"csrw mepc, t0\n"
			"mret\n"
			"1:" ::: "t0"
	);
}

__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	initialize_mstatus();
	delegate_traps();
	enter_supervisor_mode();
	exit(main(1, argv));
	__builtin_unreachable();
}
