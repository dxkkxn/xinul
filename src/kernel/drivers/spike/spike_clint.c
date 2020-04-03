#include "drivers/clint.h"

clint_device_t spike_clint = {
	.base_addr = 10000000,
	.clk_freq = 0x2000000
};
