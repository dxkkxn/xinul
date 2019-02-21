#pragma once

#include "stdint.h"

#include "device.h"

#define csr_read(csr)														\
({																			\
	register uint64_t __v;													\
	__asm__ __volatile__ ("csrr %0, " #csr : "=r" (__v) : : "memory");		\
	__v;																	\
})

#define csr_write(csr, value)												\
({																			\
	register uint64_t __v = (uint64_t)(value);								\
	__asm__ __volatile__ ("csrw " #csr ", %0" : : "rK" (__v) : "memory");	\
})

// value is a bit mask that specifies bit position to be set (high bit)
#define csr_set(csr, value)													\
({																			\
	register uint64_t __v = (uint64_t)(value);								\
	__asm__ __volatile__ ("csrs " #csr ", %0" : : "rK" (__v) : "memory");	\
})

// value is a bit mask that specifies bit position to be cleared (high bit)
#define csr_clear(csr, value)												\
({																			\
	register uint64_t __v = (uint64_t)(value);								\
	__asm__ __volatile__ ("csrc " #csr ", %0" : : "rK" (__v) : "memory");	\
})

static inline void wfi() { __asm__ __volatile__ ("wfi" ::: "memory"); }

#define CLINT_MTIME 0xbff8
#define CLINT_MTIMECMP 0x4000

#define get_mtime() (*(uint64_t*)((uint64_t)clint_dev->base_addr + CLINT_MTIME))

#define get_mtimecmp() (*(uint64_t*)((uint64_t)clint_dev->base_addr + CLINT_MTIMECMP))

#define set_mtimecmp(value)													\
({																			\
	*(uint64_t*)(CLINT_BASE + CLINT_MTIMECMP) = (uint64_t)(value);			\
})

