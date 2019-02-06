/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Shared memory between processes.
 */

#include "stddef.h"
#include "hash.h"
#include "hmm.h"
#include "mapper.h"
#include "process.h"
#include "scheduler.h"
#include "mem.h"
#include "pmm.h"
#include "vmm.h"
#include "shm.h"
#include "string.h"

static hash_t map;

void shm_init(void)
{
        assert(hash_init_string(&map) == 0);
}

void *shm_create(const char *hash)
{
        struct shm_handle *handle;
        void *frame;

        assert(hash != NULL);
        handle = NULL;

        /* Check that the handle do not exists */
        if (hash_isset(&map, (void*)hash)) {
                return NULL;
        }

        /* Get the current process */
        process_t *p = process_get(sched_get_active_pid());
        assert(p != NULL);

        /* No mapping, let's go ! */
        if (!p->shm_handle) {
                int hash_len = strlen(hash);
                handle = mem_alloc(sizeof(*handle));
                assert(handle != NULL);
                /* Get a frame */
                frame = hmm_frame_retain();
                if (frame == NULL) goto noframe;
                /* Map it */
                mapper_map(p->page_dir, (void*)PROCESS_SHM, frame, 1, 1, 0, 1);
				flush_tlb();
                /* Fill process handle */
                handle->frame = frame;
                handle->ref_count = 1;
                handle->hash = mem_alloc(hash_len + 1);
                assert(handle->hash != NULL);
                strncpy(handle->hash, hash, hash_len);
                handle->hash[hash_len] = '\0';
                p->shm_handle = handle;
                /* Register it */
                assert(hash_set(&map, handle->hash, handle) == 0);
        }

        return (void*)PROCESS_SHM;
noframe:
        mem_free(handle, sizeof(*handle));
        return NULL;
}

void *shm_acquire(const char *hash)
{
        struct shm_handle *handle;

        if ((handle = hash_get(&map, (void*)hash, NULL)) == NULL) {
                return NULL;
        }
        assert(handle->frame != NULL);
        assert(handle->ref_count >= 1);

        /* Get the current process */
        process_t *p = process_get(sched_get_active_pid());
        assert(p != NULL);

        if (p->shm_handle == NULL) {
                mapper_map(p->page_dir, (void*)PROCESS_SHM, handle->frame, 1, 1, 0, 1);
				flush_tlb();
                handle->ref_count++;
                p->shm_handle = handle;
        }

        return (void*)PROCESS_SHM;
}

void shm_release(const char *hash)
{
        assert(hash != NULL);
        /* Get the current process */
        process_t *p = process_get(sched_get_active_pid());
        shm_cleanup(p, hash);
}

void shm_cleanup(process_t *p, const char *hash)
{
        struct shm_handle *handle;

        assert((handle = hash_get(&map, (void*)hash, NULL)) != NULL);
        assert(handle->frame != NULL);
        assert(handle->ref_count >= 1);

        assert(p != NULL);
        assert(p->shm_handle != NULL);

        mapper_unmap(p->page_dir, (void*)PROCESS_SHM);
		flush_tlb();
        p->shm_handle = NULL;
        handle->ref_count--;

        if (handle->ref_count == 0) {
                hmm_frame_release(handle->frame);
                assert(hash_del(&map, (void*)hash) == 0);
                mem_free(handle->hash, strlen(hash)+1);
                mem_free(handle, sizeof(*handle));
        }

}
