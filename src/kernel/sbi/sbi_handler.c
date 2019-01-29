#include "machine_trap.h"
#include "encoding.h"
#include "csr.h"
#include "machine.h"
#include "sbi.h"
#include "device.h"

void handle_mtimer_interrupt() {
	// trigger a supervisor timer interrupt
	csr_set(mip, MIP_STIP);
	// disable machine timer interrupt (or else we would go back here right
	// after the ret instruction)
	csr_clear(mie, MIP_MTIP);
}

uint64_t handle_sbi_call(
		uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
	csr_write(mepc, csr_read(mepc)+4);
	switch(call_no) {
		case SBI_SET_TIMER:
#if 0
			uint64_t curtime = get_mtime();
			uint64_t delta_ms = arg0;
			uint64_t clk_freq = clint_dev->clk_freq;
			uint64_t new_irq_time = curtime + delta_ms * (clk_freq / 1000);
			set_mtimecmp(new_irq_time);
#endif
			set_mtimecmp(get_mtime() + arg0 * (clint_dev->clk_freq / 1000));
			csr_set(mie, MIP_MTIP);
			break;
		default:
			die("machine mode: sbi call %ld\n", call_no);
			break;
	}

	return 0;
}
