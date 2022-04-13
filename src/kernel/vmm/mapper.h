/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Memory mapper.
 * A way to create a link between a frame and a page.
 */

#ifndef _MAPPER_H_
#define _MAPPER_H_

#include "stdint.h"
#include "vmm.h"


/**
 * Create mapping between frame and page on page_dir using the provided flags.
 *
 * @param page_dir the page directory where to do the mapping.
 * @param page the page (linear address) to map.
 * @param frame the frame to map in linear space.
 * @param flags the flag used to tag the mapping.
 */
void mapper_map(pagetable_t pdirectory, void *page, void *frame, uint8_t readable, uint8_t writable, uint8_t executable, uint8_t user);

/**
 * Get the frame corresponding to the specified virtual address.
 *
 * @param satp register to find page_dir the page directory to modify.
 * @param address the address to unmap.
 */
 void *mapper_getmap(pagetable_t pgdir, void *address);

/**
 * Unmap the specified virtual address.
 *
 * @param satp register to find page_dir the page directory to modify.
 * @param address the address to unmap.
 */
 void *mapper_unmap(pagetable_t pgdir, void *address);

#endif /* _MAPPER_H_ */
