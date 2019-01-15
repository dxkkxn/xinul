/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Process Memory Mapper.
 * Describes the memory map of a process.
 */

#include "pmm.h"
#include "virtual_memory.h"
#include "hmm.h"
#include "mapper.h"
#include "process.h"
#include "stddef.h"
#include "stdio.h"
#include "mem.h"
#include "string.h"


struct pmm_varea {
	struct vmm_area *varea;
	pagetable_t directory;
};

// The base kernel page directory
pagetable_t kernel_pgdir;
// Giga page to map 1:1 kernel
struct pte kernel_gigapage;

void pmm_create_basic_directory(process_t *p)
{
	pagetable_t new_pgdir;

	assert(p != NULL);

	/* Allocate the newdirectory */
	new_pgdir = hmm_frame_retain();
	assert(new_pgdir != NULL);

	/* Map the minimal kernel space */
	new_pgdir[2] = kernel_gigapage;

	/* Set the directory */
	satp_csr satp;
	satp.field.MODE = SATP_MODE_SV39;
	satp.field.ASID = p->pid;
	satp.field.PPN = (uint64_t) new_pgdir >> 12;
	p->context.satp = satp.reg;
	p->page_dir = new_pgdir;
}

static struct pmm_varea *pmm_create_space(process_t *p, void *vstart, unsigned int size, int flags)
{
	struct vmm_area *varea;
	struct pmm_varea *parea;

	assert(p->page_dir != NULL);

	varea = vmm_area_create(vstart, size, p->page_dir, flags);
	assert(varea != NULL);
	parea = (struct pmm_varea *) mem_alloc(sizeof(struct pmm_varea));
	parea->varea = varea;
	parea->directory = p->page_dir;

	return parea;
}

struct pmm_varea *pmm_create_kstack(process_t *p, void *vstart, unsigned int size)
{
	assert(p != NULL);
	assert(size != 0);
	return pmm_create_space(p, vstart, size, VMM_AREA_KERNEL | VMM_AREA_RW);
}

struct pmm_varea *pmm_create_ustack(process_t *p, void *vstart, unsigned int size)
{
	assert(p != NULL);
	assert(size != 0);
	return pmm_create_space(p, vstart, size, VMM_AREA_USER | VMM_AREA_RW);
}

struct pmm_varea *pmm_create_ucode(process_t *p, void *vstart, unsigned int size)
{
	assert(p != NULL);
	assert(size != 0);
	return pmm_create_space(p, vstart, size, VMM_AREA_USER | VMM_AREA_RW);
}

struct pmm_varea *pmm_create_udata(process_t *p, void *vstart, unsigned int size)
{
	assert(p != NULL);
	assert(size != 0);
	return pmm_create_space(p, vstart, size, VMM_AREA_USER | VMM_AREA_RW);
}

void pmm_destroy_area(struct pmm_varea *area)
{
	assert(area != NULL);
	assert(area->varea != NULL);

	vmm_area_free(area->varea);
	mem_free(area, sizeof(struct pmm_varea));
}

void pmm_destroy_directory(process_t *p)
{
	pagetable_t pgdir;

	assert(p != NULL);
	assert(p->page_dir != NULL);

	/* Map the directory to be able to modify it */
	pgdir = p->page_dir;

	/* Release all page table frames */
	for (int i = 0; i < 512; i++) {
		if (pgdir[i].V == 1) {
			// ON regarde si c'est un noeud
			if (pgdir[i].R == 0 && pgdir[i].W == 0 && pgdir[i].X == 0) {
				// On parcours le directory de niveau 2
				pagetable_t pgdir2 = get_pagetable_entry_target_address(pgdir + i);
				for (int j = 0; j < 512; j++) {
					if (pgdir2[j].V == 1
						&& pgdir2[j].R == 0 && pgdir2[j].W == 0 && pgdir2[j].X == 0
						&& hmm_is_one_of_ours(get_pagetable_entry_target_address(pgdir2 + j))) {
						hmm_frame_release(get_pagetable_entry_target_address(pgdir2 +j));
					} // end if
					pgdir2[j].V = 0;
				} // end for entries in pgdir2
				// Free pgdir2
				if (hmm_is_one_of_ours(get_pagetable_entry_target_address(pgdir +i))) {
					hmm_frame_release(get_pagetable_entry_target_address(pgdir +i));

				}
			} // end if entry in pgdir is a node
			pgdir[i].V = 0;
		} // end if entry is valid
	} // end for entries in pgdir

	/* Release the page dir frame */
	hmm_frame_release(pgdir);
	p->page_dir = NULL;
}

