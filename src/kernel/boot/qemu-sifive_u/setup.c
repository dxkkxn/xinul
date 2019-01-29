// See LICENSE for license details.

#include "stdint.h"
#include "auxval.h"
#include "device.h"
#include "encoding.h"
#include "csr.h"

auxval_t __auxv[] = {
    { UART0_CLOCK_FREQ,         32000000   },
    { UART0_BAUD_RATE,          115200     },
    { SIFIVE_UART0_CTRL_ADDR,   0x10013000 },
    { SIFIVE_TEST_CTRL_ADDR,    0x100000   },
    { 0, 0 }
};

void arch_setup()
{
    register_console(&console_sifive_uart);
    register_poweroff(&poweroff_sifive_test);
	register_clint(&sifive_clint);
	register_plic(&sifive_plic);

	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// PPM configuration
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t) 1U << 53) - 1;
	// allow access to all of the memory for everyone
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);
}
