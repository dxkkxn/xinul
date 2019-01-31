#include "supervisor_trap.h"

#include "stdio.h"
#include "encoding.h"
#include "csr.h"
#include "machine.h"
#include "machine_trap.h"
#include "scheduler.h"
#include "kbd.h"
#include "timer.h"

//variable globale pour sauvgarder le SP kernel lors de l'éxécution en mode user
uint64_t sav_stack_kernel = 0;


void strap_handler(uintptr_t scause, uintptr_t sepc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
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
						(uint64_t) scause & ~INTERRUPT_CAUSE_FLAG, (void *) sepc
				);
				break;
		}
	} else {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
			default:
				die(
						"supervisor mode: unhandable exception %ld @ %p",
						(uint64_t) scause, (void *) sepc
				);
				break;
		}
	}
}
