/*
 * Projet PCSEA RISC-V
 *
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "assert.h"
#include "encoding.h"
#include "riscv.h"

#include "trap.h"
#include "context.h"
#include "kbd.h"
#include "timer.h"

void strap_handler(uint64_t scause, uint64_t sepc, struct caller_context *cc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
			case intr_s_timer:
//				 Set a new timer interrupt
				handle_stimer_interrupt();
				break;
			case intr_s_external:
				keyboard_handler();
				csr_clear(sip, MIP_SEIP);
				break;
			default:
				die(
						"supervisor mode: unhandable interrupt %ld : %s @ 0x%lx",
						scause & ~INTERRUPT_CAUSE_FLAG, interruption_names[scause & ~INTERRUPT_CAUSE_FLAG], sepc
				);
		}
	} else {
				die(
						"supervisor mode: unhandable exception %ld @ 0x%lx",
						scause, sepc
				);
	}
}
