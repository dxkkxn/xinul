// See LICENSE for license details.

#include "arch/riscv/encoding.h"
#include "machine.h"
#include "supervisor.h"

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */


int main(int argc, char **argv);

// this function is called by entry.S
// Only the interrupt vector is set up and we are still in machine mode
__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	
	// supervisor set-up and switch of mode (M to S)
	delegate_traps();
	enter_supervisor_mode();

	// supervisor entry point
	exit(main(1, argv));
	__builtin_unreachable();
}
