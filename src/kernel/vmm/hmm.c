/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 * Copyright (C) 2019 - Mathieu Barbe <mathieu@kolabnow.com>
 *
 * Hardware Memory Manager
 * Manages hardware memory, ie available frames.
 */

#include "stddef.h"
#include "stdlib.h"
#include "mem.h"
#include "queue.h"
#include "string.h"
#include "assert.h"
#include "stdint.h"


struct frame_elem {
	int priority;
	link linker;
};

static void *frame_start;
static void *frame_end;

#define FRAME_SIZE_MASK         0xFFFFF000
#define FRAME_SIZE              0x1000

/** Free frames list */
static LIST_HEAD(free_frames);
/** Free frames count */
static unsigned int frame_count;


void *hmm_frame_retain(void)
{
	if (frame_count == 0)
		assert(0 && "No frame available");
	void *f = queue_out(&free_frames, struct frame_elem, linker);
	memset(f, 0, FRAME_SIZE);
	frame_count--;
	return f;
}

void hmm_frame_release(void *frame)
{
	assert(frame != NULL);
	assert(((unsigned long) frame & FRAME_SIZE_MASK) == (unsigned long) frame);

	// Il faut mêtre à zéro tout les champ de la struct
	memset(frame, 0, sizeof(struct frame_elem));
	queue_add(frame, &free_frames,
	struct frame_elem, linker, priority);
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

void hmm_init(void *start, void *end)
{
	assert(((unsigned long) end & FRAME_SIZE_MASK) == (unsigned long) end);
	assert((unsigned long) start < (unsigned long) end);
	frame_count = 0;

	for (void * frame_start = start; frame_start < end; frame_start += FRAME_SIZE) {
		hmm_frame_release(frame_start);
	}
	frame_start = start;
	frame_end = end;
}



int hmm_is_one_of_ours(void *frame)
{
	return (((uint64_t)frame_start <= (uint64_t) frame) && ((uint64_t) frame < (uint64_t)frame_end));
}
