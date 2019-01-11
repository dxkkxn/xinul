/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Hardware Memory Manager
 * Manages hardware memory, ie available frames.
 */

#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "mem.h"
#include "queue.h"
#include "string.h"

#include "machine.h"

#define FRAME_SIZE_MASK         0xFFFFF000
#define FRAME_SIZE              0x1000

/** Describes a physical frame */
struct frame_descriptor {
        void *address;
        unsigned int order;
        link list;
};

/** Free frames list */
static LIST_HEAD(free_frames);
/** Free frames count */
static unsigned int frame_count;
/** Frame limits */
static unsigned int frame_start;
static unsigned int frame_end;

/** Convenience list utils */
#define FREE_LIST_ADD(element)                                                 \
    queue_add((element), &free_frames, struct frame_descriptor, list, order)
#define FREE_LIST_DEL(element)                                                 \
    queue_del((element), list)
#define FREE_LIST_GET()                                                        \
    queue_out(&free_frames, struct frame_descriptor, list)

void hmm_init(void *start, void *end)
{
        char *current;
        assert(start != NULL && end != NULL);
        assert(((unsigned long)start & FRAME_SIZE_MASK) == (unsigned long)start);
        assert(((unsigned long)end & FRAME_SIZE_MASK) == (unsigned long)end);
        assert((unsigned long)start < (unsigned long)end);

        frame_count = 0;
        for (current = (char*)end - 0x1000;
             current >= (char*)start;
             current -= 0x1000)
        {
                struct frame_descriptor *local;

                local = mem_alloc(sizeof (struct frame_descriptor));
                assert(local != NULL);
                local->address = current;
                local->order = (unsigned long)current;
                FREE_LIST_ADD(local);
                frame_count++;
        }
        frame_start = (unsigned long)start;
        frame_end = (unsigned long)end;
}

void* hmm_frame_retain(void)
{
        void *frame = NULL;
        struct frame_descriptor *fd;

        fd = FREE_LIST_GET();
        if (fd != NULL) {
                frame = fd->address;
                mem_free(fd, sizeof(*fd));
                frame_count--;
        }
        memset(frame, 0, 4096);
        return frame;
}

void hmm_frame_release(void *frame)
{
        struct frame_descriptor *local;

        assert(frame != NULL);
        assert(((unsigned long)frame & FRAME_SIZE_MASK) == (unsigned long)frame);

        local = mem_alloc(sizeof(*local));
        assert(local != NULL);
        local->address = frame;
        FREE_LIST_ADD(local);
        frame_count++;
}

unsigned int hmm_frame_count(void)
{
        return frame_count;
}

unsigned int hmm_free_memory(void)
{
        return frame_count * FRAME_SIZE;
}

int hmm_is_one_of_ours(void *frame)
{
        return ((frame_start <= (unsigned long)frame) && ((unsigned long)frame < frame_end));
}
