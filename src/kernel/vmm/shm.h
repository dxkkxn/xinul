#ifndef _SHM_H_
#define _SHM_H_

struct shm_handle {
        void *frame;
        int ref_count;
        char *hash;
};

void shm_init();

void *shm_create(const char*);
void *shm_acquire(const char*);
void shm_release(const char*);
void shm_cleanup(process_t *, const char*);

#endif
