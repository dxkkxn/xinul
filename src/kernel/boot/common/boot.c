// See LICENSE for license details.

#include "arch/riscv/encoding.h"
#include "machine.h"
#include "csr.h"

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */


int main(int argc, char **argv);

__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	exit(main(1, argv));
	__builtin_unreachable();
}
