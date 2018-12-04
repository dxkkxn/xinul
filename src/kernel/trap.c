#include <machine.h>
#include "trap.h"
#include "handlers.h"

#define INTERRUPT_CAUSE_FLAG (1UL << 63)

const char * riscv_excp_names[16] = {
	"misaligned_fetch",
	"fault_fetch",
	"illegal_instruction",
	"breakpoint",
	"misaligned_load",
	"fault_load",
	"misaligned_store",
	"fault_store",
	"user_ecall",
	"supervisor_ecall",
	"hypervisor_ecall",
	"machine_ecall",
	"exec_page_fault",
	"load_page_fault",
	"reserved",
	"store_page_fault"
};

const char * riscv_intr_names[16] = {
	"u_software",
	"s_software",
	"h_software",
	"m_software",
	"u_timer",
	"s_timer",
	"h_timer",
	"m_timer",
	"u_external",
	"s_external",
	"h_external",
	"m_external",
	"reserved",
	"reserved",
	"reserved",
	"reserved"
};


void trap_handler(uintptr_t* regs, uintptr_t mcause, uintptr_t mepc)
{
	if (mcause & INTERRUPT_CAUSE_FLAG) {
		switch (mcause & ~INTERRUPT_CAUSE_FLAG) {
		case intr_m_timer:
			handle_mtimer_interrupt();
			break;
		default:
			die(
					"machine mode: unhandlable trap %d @ %p",
					(unsigned int)mcause, (void *)mepc
			);
			break;
		}
	}
	else {
		die(
				"machine mode: unhandlable trap %d @ %p",
				(unsigned int)mcause, (void *)mepc
		);
	}
}
