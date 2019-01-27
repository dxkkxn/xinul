// See LICENSE for license details.

#include "encoding.h"
#include "machine.h"
#include "supervisor.h"

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */


int main();

// this function is called by entry.S
// Only the interrupt vector is set up and we are still in machine mode
__attribute__((noreturn)) void boot_riscv()
{
	arch_setup();
	uint64_t* plic_addr = (uint64_t*) 0xc002000;
	for(int i=0; i<10; i++, plic_addr++) {
		*plic_addr = -1;
	}
	*((uint64_t*)0xc200000) = 0;

	*((uint64_t*)0xc00000c) = 1;
	
	// supervisor set-up and switch of mode (M to S)
	delegate_traps();
	enter_supervisor_mode();
	plic_addr = (uint64_t*) 0xc002080;
	for(int i=0; i<10; i++, plic_addr++) {
		*plic_addr = -1;
	}
	*((uint64_t*)0xc201000) = 0;

	exit(main());
	__builtin_unreachable();
}
