#include "drivers/clint.h"

clint_device_t sifive_clint = {
		.clk_freq = 10000000,
		.base_addr = 0x2000000
};
