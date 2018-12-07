#ifndef KERNEL_RISCV_HANDLERS_H
#define KERNEL_RISCV_HANDLERS_H

#include "stdint.h"

void handle_mtimer_interrupt();

uint64_t handle_sbi_call(
		uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2
);

#endif  /* KERNEL_RISCV_HANDLERS_H */
