#include "handlers.h"
#include "csr.h"
#include "irq.h"
#include "stdio.h"

void handle_mtimer_interrupt() {
	// trigger a supervisor timer interrupt
	// WARINING: this is useless for now
	csr_set(mip, MIX_STI);
	set_mtimecmp(get_mtime() + 1000000);
}
