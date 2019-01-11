/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Hardware Memory Manager
 * Manages hardware memory, ie available frames.
 */

#ifndef _HMM_H_
#define _HMM_H_

/**
 * Build a list of all free frames.
 *
 * @param start the address of the first free frame.
 * @param end the addres of the end of the harware memory.
 *
 * @note start and end MUST be algned on 4K.
 */
void hmm_init(void *start, void *end);

/**
 * Retain a free frame.
 *
 * @return a reference on the beggining of the free frame.
 */
void* hmm_frame_retain(void);

/**
 * Free a previously used frame.
 * @param frame address of the frame to release.
 *
 * @note frame MUST be 4K aligned !
 */
void hmm_frame_release(void *frame);

/**
 * Retrieve the number of free frames.
 *
 * @return the number of free frames registered in the manager.
 */
unsigned int hmm_frame_count(void);

/**
 * Retrieve the amount of free memory.
 *
 * @return the amount of free memory in Bytes.
 */
unsigned int hmm_free_memory(void);

/**
 * Tell if the given frame is one of ours.
 *
 * @return true if the frame is in our space.
 */
int hmm_is_one_of_ours(void *frame);
#endif /* _HMM_H_ */
