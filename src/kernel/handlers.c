#include "handlers.h"
#include "irq.h"
#include "sbi.h"
#include "csr.h"
#include "machine.h"

void handle_mtimer_interrupt() {
	// trigger a supervisor timer interrupt
	csr_set(mip, MIX_STI);
	// disable machine timer interrupt (or else we would go back here right
	// after the ret instruction)
	csr_clear(mie, MIX_MTI);
}

uint64_t handle_sbi_call(
		uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
	csr_write(mepc, csr_read(mepc)+4);
	switch(call_no) {
		case SBI_SET_TIMER:
			set_mtimecmp(get_mtime() + arg0);
			csr_set(mie, MIX_MTI);
			break;
		default:
			die("machine mode: sbi call %d\n", call_no);
			break;
	}

	return 0;
}
