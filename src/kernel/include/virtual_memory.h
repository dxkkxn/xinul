// See LICENSE for license details.

#pragma once

#include "stdint.h"

#define PAGE_SIZE 4096
#define NBR_PAGE_ENTRIES 512;

// Physical Page Number (Frame Number) 44 bits
//typedef void* ppn_t;

// CSR
struct satp_csr {
	uint64_t PPN:44;
	uint64_t ASID:16;
	uint64_t MODE:4;
};

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

typedef struct pte * pagetable_t;

// Prototype
int8_t init_virtual_memory();
