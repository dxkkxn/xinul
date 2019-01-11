/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 * Copyright (C) 2019 - Mathieu Barbe <mathieu@kolabnow.com>
 *
 * Memory mapper.
 * A way to create a link between a frame and a page.
 */

#include "stddef.h"

#include "machine.h"
#include "hmm.h"
#include "virtual_memory.h"
#include "mapper.h"


void mapper_map(satp_csr satp, void *page, void *frame,
				uint8_t readable, uint8_t writable, uint8_t executable, uint8_t user)
{
	pagetable_t pdirectory, pdirectory2, ptable;
	uint16_t pdi, pd2i, pti;

	// Check if virtual address is not under 1 and 2 GO
	assert(!((uint64_t) page >= 0x80000000 && (uint64_t) page < 0x90000000));

	pdirectory = get_directory(satp);
	assert(pdirectory != NULL);
	/* Set dir/table indexes */
	pdi = (uint64_t) page >> 30;
	pd2i = (uint64_t) page >> 21 & 0x1FF;;
	pti = ((uint64_t) page >> 12) & 0x3FF;

	/* Check that the page table is available */
	struct pte *pde = &(pdirectory[pdi]);
	if (pde->V == 0) {
		/* No page table, create it ! */
		pdirectory2 = hmm_frame_retain();
		assert(pdirectory2 != NULL);
		/* Insert in the directory */
		init_pagetable_entry_node(pde, pdirectory2);
	} else {
		pdirectory2 = get_pagetable_entry_target_address(pde);
		assert(pdirectory2 != NULL);
	}


	/* Check that the page table is available */
	struct pte *pd2e = &(pdirectory2[pd2i]);
	if (pd2e->V == 0) {
		/* No page table, create it ! */
		ptable = hmm_frame_retain();
		assert(ptable != NULL);
		/* Insert in the directory */
		init_pagetable_entry_node(pd2e, ptable);
	} else {
		ptable = get_pagetable_entry_target_address(pd2e);
		assert(ptable != NULL);
	}

	/* Finally map the */
	struct pte *pte = &(ptable[pti]);
	init_pagetable_entry(pte, frame, readable, writable, executable, user);
}
/*
void *mapper_unmap(void *page_dir, void *address)
{
        (uint64_t) int pdi, pti;
        (uint64_t) long *pdirectory;
        (uint64_t) long *ptable;
        (uint64_t) long frame_entry;

        assert(page_dir != NULL);
        pdirectory = page_dir;

        //Set dir/table indexes 
        pdi = ((uint64_t))(address) >> 22;
        pti = (((uint64_t))(address) >> 12) & 0x3FF;

        ptable = ((uint64_t) long*)(pdirectory[pdi] & 0xFFFFF000u);
        frame_entry = ((uint64_t) long)(ptable[pti]);
        ptable[pti] = 0u;

        return (void*)(frame_entry & 0xFFFFF000u);
}
*/
