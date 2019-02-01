#include <machine.h>
#include "machine_trap.h"
#include "trap.h"
#include "sbi.h"

#include "csr.h"
#include "context.h"

struct caller_context m_caller_context;


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


void mtrap_handler(struct caller_context regs, uintptr_t mcause, uintptr_t mepc)
{
	if (mcause & INTERRUPT_CAUSE_FLAG) {
		switch (mcause & ~INTERRUPT_CAUSE_FLAG) {
		case intr_m_timer:
			handle_mtimer_interrupt();
			break;
		default:
			die(
					"machine mode: unhandlable trap %ld @ %p", 
					(uint64_t) mcause, (void *) mepc
			);
			break;
		}
	} else {
		switch (mcause) {
		case CAUSE_SUPERVISOR_ECALL:
			// call the function with the saved registers a7, a0, a1 and a2
			handle_sbi_call(regs.a7, regs.a0, regs.a1, regs.a2);
			break;
		default:
			die(
					"machine mode: unhandlable trap %ld @ %lx", 
					(uint64_t) mcause, mepc
			);
			break;
		}
	}
}
