#ifndef ARCH_RISCV_IRQ_H
#define ARCH_RISCV_IRQ_H

#include "csr.h"

// MIX = mie or mip
#define MIX_USI 0x001
#define MIX_SSI 0x002
#define MIX_HSI 0x004
#define MIX_MSI 0x008
#define MIX_UTI 0x010
#define MIX_STI 0x020
#define MIX_HTI 0x040
#define MIX_MTI 0x080
#define MIX_UEI 0x100
#define MIX_SEI 0x200
#define MIX_HEI 0x400
#define MIX_MEI 0x800

#define MSTATUS_UIE 0x1
#define MSTATUS_SIE 0x2
#define MSTATUS_HIE 0x4
#define MSTATUS_MIE 0x8

// mstatus configuration: interruptions
#define enable_user_irq() csr_set(mstatus, MSTATUS_UIE)
#define disable_user_irq() csr_clear(mstatus, MSTATUS_UIE)
#define enable_supervisor_irq() csr_set(mstatus, MSTATUS_SIE)
#define disable_supervisor_irq() csr_clear(mstatus, MSTATUS_SIE)
#define enable_hypervisor_irq() csr_set(mstatus, MSTATUS_HIE)
#define disable_hypervisor_irq() csr_clear(mstatus, MSTATUS_HIE)
#define enable_machine_irq() csr_set(mstatus, MSTATUS_MIE)
#define disable_machine_irq() csr_clear(mstatus, MSTATUS_MIE)

// mie configuration: timer interruption
#define enable_user_timer_irq() csr_set(mie, MIX_UTI)
#define disable_user_timer_irq() csr_clear(mie, MIX_UTI)
#define enable_supervisor_timer_irq() csr_set(mie, MIX_STI)
#define disable_supervisor_timer_irq() csr_clear(mie, MIX_STI)
#define enable_hypervisor_timer_irq() csr_set(mie, MIX_HTI)
#define disable_hypervisor_timer_irq() csr_clear(mie, MIX_HTI)
#define enable_machine_timer_irq() csr_set(mie, MIX_MTI)
#define disable_machine_timer_irq() csr_clear(mie, MIX_MTI)

// mie configuration: external interruption
#define enable_user_timer_irq() csr_set(mie, MIX_UEI)
#define disable_user_timer_irq() csr_clear(mie, MIX_UEI)
#define enable_supervisor_timer_irq() csr_set(mie, MIX_SEI)
#define disable_supervisor_timer_irq() csr_clear(mie, MIX_SEI)
#define enable_hypervisor_timer_irq() csr_set(mie, MIX_HEI)
#define disable_hypervisor_timer_irq() csr_clear(mie, MIX_HEI)
#define enable_machine_timer_irq() csr_set(mie, MIX_MEI)
#define disable_machine_timer_irq() csr_clear(mie, MIX_MEI)

// mie configuration: software interruption
#define enable_user_timer_irq() csr_set(mie, MIX_USI)
#define disable_user_timer_irq() csr_clear(mie, MIX_USI)
#define enable_supervisor_timer_irq() csr_set(mie, MIX_SSI)
#define disable_supervisor_timer_irq() csr_clear(mie, MIX_SSI)
#define enable_hypervisor_timer_irq() csr_set(mie, MIX_HSI)
#define disable_hypervisor_timer_irq() csr_clear(mie, MIX_HSI)
#define enable_machine_timer_irq() csr_set(mie, MIX_MSI)
#define disable_machine_timer_irq() csr_clear(mie, MIX_MSI)


#endif /* ARCH_RISCV_IRQ_H */
