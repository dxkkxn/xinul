#ifndef ARCH_RISCV_TRAP_H
#define ARCH_RISCV_TRAP_H

#include <stdint.h>

const char * riscv_excp_names[16];
const char * riscv_intr_names[16];

enum exception_cause_flag {
	cause_instruction_address_misaligned	= 0,
	cause_instruction_access_fault			= 1,
	cause_illegal_instruction				= 2,
	cause_breakpoint						= 3,
	cause_load_address_misaligned			= 4,
	cause_load_access_fault					= 5,
	cause_store_address_misaligned			= 6,
	cause_store_access_fault				= 7,
	cause_user_ecall						= 8,
	cause_supervisor_ecall					= 9,
	cause_hypervisor_ecall					= 10,
	cause_machine_ecall						= 11,
	cause_instruction_page_fault			= 12,
	cause_load_page_fault					= 13,
	cause_store_page_fault					= 15
};

enum interruption_cause_flag {
	intr_u_software		= 0,
	intr_s_software		= 1,
	intr_h_software		= 2,
	intr_m_software		= 3,
	intr_u_timer		= 4,
	intr_s_timer		= 5,
	intr_h_timer		= 6,
	intr_m_timer		= 7,
	intr_u_external		= 8,
	intr_s_external		= 9,
	intr_h_external		= 10,
	intr_m_external		= 11,
};

void trap_handler(uintptr_t* regs, uintptr_t mcause, uintptr_t mepc);

#endif /* ARCH_RISCV_TRAP_H */
