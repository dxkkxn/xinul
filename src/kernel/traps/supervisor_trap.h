#ifndef _KERNEL_IRQ_H_
#define _KERNEL_IRQ_H_

#include "csr.h"

#define ENABLE_SUPERVISOR_INTERRUPTS() csr_set(sstatus, MSTATUS_SIE);
#define DISABLE_SUPERVISOR_INTERRUPTS() csr_reset(mstatus, MSTATUS_SIE);

void setup_clock_interrupts();

#endif /* _KERNEL_IRQ_H_ */
