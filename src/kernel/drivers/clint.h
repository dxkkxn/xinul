#pragma once

#define CLINT_MTIME 0xbff8
#define CLINT_MTIMECMP 0x4000

typedef struct clint_device {
	int clk_freq;
	int base_addr;
} clint_device_t;

/*
 * clint_dev global variable
 * This variable is useful for fetching base addr and clock freq informations on the CLINT of the machine.
 */
extern clint_device_t *clint_dev;

/*
 * CLINT drivers
 */
extern clint_device_t clint_none;
extern clint_device_t spike_clint;
extern clint_device_t sifive_clint;
extern clint_device_t cep_clint;

void register_clint(clint_device_t *dev);

/*
 * CLINT macros
 */

#define get_mtime() (*(uint64_t*)((uint64_t)clint_dev->base_addr + CLINT_MTIME))

#define get_mtimecmp() (*(uint64_t*)((uint64_t)clint_dev->base_addr + CLINT_MTIMECMP))

#define set_mtimecmp(value)                                                    \
({                                                                            \
    *(uint64_t*)(CLINT_BASE + CLINT_MTIMECMP) = (uint64_t)(value);            \
})

