#pragma once

#include "stdint.h"


/*
 * Prototypes
 */

// int tic; //global timer variable

void handle_mtimer_interrupt();
void set_machine_timer_interrupt(uint64_t delta_ms);
void set_supervisor_timer_interrupt(uint64_t delta_ms);
void handle_stimer_interrupt();

