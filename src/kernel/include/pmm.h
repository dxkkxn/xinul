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

/**
 * Process kernel stack.
 * @note A stack usage decreases the pointer.
 */
// todo voir si on vire ou modif
// 2,5Go
#define     PROCESS_KSTACK      0xa0000000

/******************
 *   User space   *
 ******************/

/** Process code start */
#define     PROCESS_CODE        0x40000000


/** Process heap start
 * 3Go
 * */

#define     PROCESS_HEAP        0xC0000000

/** Process shared page */
// todo a comprendre et à mettre en place
#define     PROCESS_SHM        (PROCESS_HEAP - PAGE_SIZE)

/**
 * Process stack start.
 * @note A stack usage decreases the pointer.
 * 4Go
 */
#define     PROCESS_USTACK      0x100000000


struct pmm_varea;

void pmm_create_basic_directory(process_t *p);

struct pmm_varea *pmm_create_kstack(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_ustack(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_ucode(process_t *p, void *vstart, unsigned int size);

struct pmm_varea *pmm_create_udata(process_t *p, void *vstart, unsigned int size);

void pmm_destroy_area(struct pmm_varea *area);

void pmm_destroy_directory(process_t *p);

#endif /* _PMM_H_ */
