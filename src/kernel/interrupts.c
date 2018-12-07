#include "interrupts.h"

#include "encoding.h"
#include "csr.h"
#include "machine_trap.h"
#include "machine.h"
#include "sbi.h"




void setup_clock_interrupts() {
	csr_set(sie, MIP_STIP);

	// Schedule the first interruption in 100ms
	set_mtimecmp(get_mtime() + 100 *(SPIKE_CLOCK_FREQUENCY/1000));
}

int set_next_timer_event()
{
	uint64_t delta = 1000; // ms
	// set the new mtimecmp value and activate the machine timer interruptions
	sbi_call_set_timer(delta);
}

void strap_handler(uintptr_t* regs, uintptr_t scause, uintptr_t sepc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
		case intr_s_timer:
			// Set a new timer interrupt
			set_next_timer_event();
			// Clear the interrupt flag so that the processor does not take
			// this trap again after return from interrupt
			csr_clear(sip, MIP_STIP);
			break;
		default:
			die("supervisor mode: unhandable interrupt %d @ %p", scause, sepc);
			break;
		}
	} else {
		die("supervisor mode: unhandable exception %d @ %p", scause, sepc);
	}
}
