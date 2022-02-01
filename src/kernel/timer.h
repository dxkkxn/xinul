#pragma once

#include "stdint.h"


/*
 * Prototypes
 */

void handle_mtimer_interrupt();
void set_machine_timer_interrupt(uint64_t delta_ms);
void handle_stimer_interrupt();

