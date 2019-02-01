#ifndef ARCH_RISCV_TRAP_H
#define ARCH_RISCV_TRAP_H

#include <stdint.h>

#define INTERRUPT_CAUSE_FLAG (1UL << 63)

const char * riscv_excp_names[16];
const char * riscv_intr_names[16];

#endif /* ARCH_RISCV_TRAP_H */
