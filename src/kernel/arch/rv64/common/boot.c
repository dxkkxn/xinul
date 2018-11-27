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

__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	exit(main(1, argv));
	__builtin_unreachable();
}
