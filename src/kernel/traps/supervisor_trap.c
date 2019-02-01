#include "supervisor_trap.h"
#include "trap.h"

#include "stdio.h"
#include "encoding.h"
#include "csr.h"
#include "machine.h"
#include "machine_trap.h"
#include "scheduler.h"
#include "kbd.h"
#include "timer.h"

void strap_handler(struct trap_frame *tf)
{
	if (tf->scause & INTERRUPT_CAUSE_FLAG) {
		switch (tf->scause & ~INTERRUPT_CAUSE_FLAG) {
			case intr_s_timer:
				// Set a new timer interrupt
				clock_handler();
				break;
			case intr_s_external:
				keyboard_handler();
				csr_clear(sip, MIP_SEIP);
				break;
			default:
				die(
						"supervisor mode: unhandable interrupt %ld @ %p",
						tf->scause & ~INTERRUPT_CAUSE_FLAG, (void *) tf->sepc
				);
				break;
		}
	} else {
		switch (tf->scause & ~INTERRUPT_CAUSE_FLAG) {
			default:
				die(
						"supervisor mode: unhandable exception %ld @ %p",
						tf->scause, (void *) tf->sepc
				);
				break;
		}
	}
}
