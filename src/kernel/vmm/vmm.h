// See LICENSE for license details.

#pragma once

#include "stdint.h"

#define PAGE_SIZE 4096
#define NBR_PAGE_ENTRIES 512;


enum vmm_flag {
	VMM_AREA_KERNEL = 0,
	VMM_AREA_RX = VMM_AREA_KERNEL,
	VMM_AREA_USER = 1,
	VMM_AREA_RW = 1 << 1
};

/** Virtual area descriptor. */
struct vmm_area;

// Physical Page Number (Frame Number) 44 bits
//typedef void* ppn_t;

// CSR
union satp {
	void* reg;
	uint64_t ureg;
	struct {
		uint64_t PPN:44;
		uint64_t ASID:16;
		uint64_t MODE:4;
	} field;
};
typedef union satp satp_csr;

#define SATP_MODE_OFF  0
#define SATP_MODE_SV32 1
#define SATP_MODE_SV39 8
#define SATP_MODE_SV48 9
#define SATP_MODE_SV57 10
#define SATP_MODE_SV64 11

// Page table
// Virtual Page Entry 64 bits
struct pte {
	uint64_t V:1;
	uint64_t R:1;
	uint64_t W:1;
	uint64_t X:1;
	uint64_t U:1;
	uint64_t G:1;
	uint64_t A:1;
	uint64_t D:1;
	uint64_t RSW:2;
	uint64_t PPN0:9;
	uint64_t PPN1:9;
	uint64_t PPN2:26;
	uint64_t RSSV:10;
};

typedef struct pte *pagetable_t;

// flush tlb
static inline void flush_tlb()
{
	__asm__ __volatile__("sfence.vma");
}

// Prototype
int8_t init_virtual_memory();

void init_pagetable_entry(struct pte *pte, void *frame_addr, uint8_t readable, uint8_t writable, uint8_t executable,
						  uint8_t user);

void init_pagetable_entry_node(struct pte *entry, void *pagetable);

satp_csr get_kernel_satp();

pagetable_t get_directory(satp_csr satp);

void *get_pagetable_entry_target_address(struct pte *pte);

pagetable_t get_current_directory(void);

/**
 * Create a mapped virtual area.
 *
 * @param base address of the beginning of the virtual area.
 * @param size size of the virtual area.
 * @param dir  directory where to do the mapping.
 */
struct vmm_area *vmm_area_create(void *base,
								 unsigned long size,
								 pagetable_t dir,
								 int flags);

/**
 * Release a mapped virtual area.
 *
 * @param varea the descriptor of the virtual area to free.
 */
void vmm_area_free(struct vmm_area *varea);

