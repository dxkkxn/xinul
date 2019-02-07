// See LICENSE for license details.

#include "stdint.h"
#include "string.h"
#include "auxval.h"
#include "device.h"
#include "encoding.h"
#include "csr.h"

extern char default_program[20];

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
	strncpy(default_program, "console", 20);

	// activation des compteurs de performances
	csr_write(mcounteren, -1);
	csr_write(scounteren, -1);
}
