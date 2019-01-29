// See LICENSE for license details.

#include "stdint.h"
#include "auxval.h"
#include "device.h"
#include "encoding.h"
#include "csr.h"

extern uint64_t tohost;
extern uint64_t fromhost;

auxval_t __auxv[] = {
		{RISCV_HTIF_BASE_ADDR, 0},
		{0,                    0}
};

void arch_setup()
{
	__auxv[0].val = (uintptr_t) (&tohost < &fromhost ? &tohost : &fromhost);
	register_console(&console_htif);
	register_poweroff(&poweroff_htif);


	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// PPM configuration
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t) 1U << 53) - 1;
//	 allow access to all of the memory for everyone
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);


}
