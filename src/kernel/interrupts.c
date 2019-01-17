#include "interrupts.h"

#include "encoding.h"
#include "csr.h"
#include "machine_trap.h"
#include "machine.h"
#include "sbi.h"
#include "scheduler.h"
#include "syscall_handler.h"

void setup_clock_interrupts()
{
	csr_set(sie, MIP_STIP);

	// Schedule the first interruption in 100ms
	set_mtimecmp(get_mtime() + 100 * (SPIKE_CLOCK_FREQUENCY / 1000));
}

void set_next_timer_event()
{
	uint64_t delta = 100; // ms
	// set the new mtimecmp value and activate the machine timer interruptions
	sbi_call_set_timer(delta);
}

void strap_handler(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, uint64_t syscall_no,
				   uintptr_t scause, uintptr_t sepc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
			case intr_s_timer:
				// Set a new timer interrupt
				set_next_timer_event();
				// Clear the interrupt flag so that the processor does not take
				// this trap again after return from interrupt
				csr_clear(sip, MIP_STIP);
				schedule();
				break;
			default:
				die(
						"supervisor mode: unhandable interrupt %ld @ %p",
						(uint64_t) scause, (void *) sepc
				);
				break;
		}
	} else {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
			case cause_user_ecall :
				syscall_handler(syscall_no, arg0, arg1, arg2, arg3, arg4);
				die("die after syscall to test\n");
				break;
			default:
				die(
						"supervisor mode: unhandable exception %ld @ %p",
						(uint64_t) scause, (void *) sepc
				);
				break;
		}
	}
}
