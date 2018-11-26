// See LICENSE for license details.

#include "arch/riscv/encoding.h"
#include "machine.h"
#include "csr.h"

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

void enter_supervisor_mode()
{
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t)1U << 53)-1;
	csr_set(mstatus, MSTATUS_MPP_MASK_S);
	csr_clear(mstatus, MSTATUS_MPIE_MASK);
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	csr_write(mepc, main);
	__asm__ __volatile__ ("mret");
}

__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	//exit(main(1, argv));
	enter_supervisor_mode();
	__builtin_unreachable();
}
