// See LICENSE for license details.

#include "stdint.h"
#include "string.h"
#include "auxval.h"
#include "device.h"
#include "encoding.h"
#include "csr.h"

extern uint64_t tohost;
extern uint64_t fromhost;
extern char default_program[20];

auxval_t __auxv[] = {
		{RISCV_HTIF_BASE_ADDR, 0},
		{0,                    0}
};

void arch_setup()
{
	__auxv[0].val = (uintptr_t) (&tohost < &fromhost ? &tohost : &fromhost);
	register_console(&console_htif);
	register_poweroff(&poweroff_htif);
	register_clint(&spike_clint);
	strncpy(default_program, "autotest", 20);

	// activation des compteurs de performances
	csr_write(mcounteren, -1);
	csr_write(scounteren, -1);
}
