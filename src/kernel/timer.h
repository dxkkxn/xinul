#pragma once

#include "stdint.h"

#define CLK_IT_FREQ 100


/*
 * Prototypes
 */

void set_machine_timer_interrupt(uint64_t delta_ms);
void handle_mtimer_interrupt();
void handle_stimer_interrupt();

void wait_clock(unsigned long clock);
int clock_free_processes();

void sleep(unsigned long sec);
void sleepms(unsigned long ms);
void do_for_seconds(int sec, void (*callback)());
void afficher_horloge();
void clock_init();
void clock_settings(unsigned long *quartz, unsigned long *ticks);
unsigned long current_clock(void);
