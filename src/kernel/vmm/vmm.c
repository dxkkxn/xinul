/*
 * Ensimag - Projet système
 * Copyright (C) 2019 - Mathieu Barbe <mathieu@kolabnow.com>
 *
 * Virtual memory manager
 */

#include "stdio.h"
#include "encoding.h"
#include "stdlib.h"
#include "assert.h"
#include "mem.h"
#include "riscv.h"

#include "vmm.h"
#include "hmm.h"
#include "mapper.h"

// The base kernel page directory, fetch pmm.c
extern pagetable_t kernel_pgdir;
extern struct pte kernel_gigapage;
extern struct pte io_gigapage;

static satp_csr kernel_satp;


struct vmm_area {
	void *base;
	unsigned int size;
	pagetable_t directory;
};

bool is_virtual_memory_enable()
{
	satp_csr satp = {.ureg = csr_read(satp)};
	if (satp.field.MODE == SATP_MODE_SV39)
		return true;
	else return false;
}

/* init virtual memory
* returns -1 if a error occurred.
*/
int8_t init_virtual_memory()
{
	kernel_pgdir = hmm_frame_retain();
	assert(kernel_pgdir != NULL);

	// Création d'une feuille giga page pour maper tout le kernel d'un coup à 1Go
	kernel_gigapage.V = 1;
	kernel_gigapage.R = 1;
	kernel_gigapage.W = 1;
	kernel_gigapage.X = 1;
	kernel_gigapage.U = 0;
	kernel_gigapage.G = 1;
	kernel_gigapage.A = 1;
	kernel_gigapage.D = 1;
	kernel_gigapage.RSW = 0;
	kernel_gigapage.PPN0 = 0;
	kernel_gigapage.PPN1 = 0;
	kernel_gigapage.PPN2 = 2;
	kernel_gigapage.RSSV = 0;
	kernel_pgdir[2] = kernel_gigapage;

	io_gigapage = kernel_gigapage;
	io_gigapage.PPN0 = 0;
	io_gigapage.PPN1 = 0;
	io_gigapage.PPN2 = 0;
	kernel_pgdir[0] = io_gigapage;

	// Configure satp csr
	kernel_satp.field.MODE = SATP_MODE_SV39;
	kernel_satp.field.ASID = 0;
	kernel_satp.field.PPN = (uint64_t) kernel_pgdir >> 12;

	csr_write(satp, kernel_satp.reg);

	// On authorise le superviseur à accéder au page user (Voir 4.3.1 Addressing and Memory Protection ISA Priv)
	csr_set(sstatus, SSTATUS_SUM);

	return 0;
}


satp_csr get_kernel_satp()
{
	return kernel_satp;
}


void init_pagetable_entry(struct pte *pte, void *frame_addr, uint8_t readable, uint8_t writable, uint8_t executable,
						  uint8_t user)
{
	pte->V = 1;
	pte->R = readable;
	pte->W = writable;
	pte->X = executable;
	pte->U = user;
	pte->G = 1;
	pte->A = 1;
	pte->D = 1;
	pte->RSW = 0;
	pte->PPN0 = (uint64_t) frame_addr >> 12;
	pte->PPN1 = (uint64_t) frame_addr >> 21;
	pte->PPN2 = (uint64_t) frame_addr >> 30;
}

void init_pagetable_entry_node(struct pte *entry, void *pagetable)
{
	init_pagetable_entry(entry, pagetable, 0, 0, 0, 0);
}

void *get_pagetable_entry_target_address(struct pte *pte)
{
	return (void *) ((uint64_t) (pte->PPN2) << 30 | (uint64_t) (pte->PPN1) << 21 | (uint64_t) (pte->PPN0) << 12);
}

pagetable_t get_directory(satp_csr satp)
{
	return (pagetable_t) ((uint64_t) satp.field.PPN << 12);
}

pagetable_t get_current_directory(void)
{
	satp_csr satp;
	satp.reg = (void *) csr_read(satp);
	return get_directory(satp);
}

struct vmm_area *vmm_area_create(void *base,
								 unsigned long size,
								 pagetable_t dir,
								 int flags)
{
	// todo fabriquer un système de flag avec des enum pour les droits contenu dans les entrées
	struct vmm_area *varea;

	assert(base != NULL);
	assert(size != 0);
	assert(dir != NULL);
	assert(((uint64_t) base & 0xFFFFFFFFFFFFF000) == (uint64_t) base);
	assert((size & 0xFFFFF000) == size);

	uint8_t readable = 1;
	uint8_t executable = 1;
	uint8_t user = 0;
	if ((flags & VMM_AREA_USER) == VMM_AREA_USER) {
		user = 1;
	}
	uint8_t writable = 0;
	if ((flags & VMM_AREA_RW) == VMM_AREA_RW) {
		writable = 1;
	}

	for (uint64_t address = (uint64_t) base;
		 address < (uint64_t) base + size;
		 address += 4096) {
		void *frame = hmm_frame_retain();
		assert(frame != NULL);
		mapper_map(dir, (void *) address, frame, readable, writable, executable, user);
	}

	varea = (struct vmm_area *) mem_alloc(sizeof(struct vmm_area));
	assert(varea != NULL);
	varea->base = base;
	varea->size = size;
	varea->directory = dir;

	return varea;
}

void vmm_area_free(struct vmm_area *varea)
{
	assert(varea != NULL);

	for (uint64_t address = (uint64_t) varea->base;
		 address < (uint64_t) varea->base + varea->size;
		 address += 4096) {
		void *frame = (void *) mapper_unmap(varea->directory, (void *) address);
		assert(frame != NULL);
		hmm_frame_release(frame);
	}

	mem_free(varea, sizeof(*varea));
}

