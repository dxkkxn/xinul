/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Process Memory Mapper.
 * Describes the memory map of a process.
 */

#ifndef _PMM_H_
#define _PMM_H_

#include "process.h"


/********************
 *   Kernel space   *
 *   2Go -> 0x80000000
 ********************/
#define     KERNEL_CODE 0x80000000

/******************
 *   User space   *
 ******************/

/** Process code start */

#define     PROCESS_CODE        0x40000000

/** Process heap start
 * 2,25 Go
 * */

#define     PROCESS_HEAP        0x50000000


/**
 * Process stack start.
 * @note A stack usage decreases the pointer.
 */

#define     PROCESS_USTACK      0x60000000

/** Process shared page */

#define     PROCESS_SHM        0x70000000


struct pmm_varea;

void pmm_create_basic_directory(process_t *p);

struct pmm_varea *pmm_create_kstack(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_ustack(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_ucode(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_udata(process_t *p, void *vstart, unsigned int size);

void pmm_destroy_area(struct pmm_varea *area);

void pmm_destroy_directory(process_t *p);

#endif /* _PMM_H_ */
