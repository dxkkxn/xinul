#include "timer.h"

#include "riscv.h"
#include "encoding.h"
#include "device.h"
#include "stdio.h"

#include "sbi.h"
#include "scheduler.h"

// Compteur de temps
static unsigned long timer = 0;

static link blocked_processes = LIST_HEAD_INIT(blocked_processes);

/* Affiche le temps écoulé depuis le démarrage du noyau */
#ifdef PRINT_TIME
static void print_time(void)
{
    unsigned sec = timer / CLK_IT_FREQ;
    unsigned min = sec / 60;
    unsigned hrs = min / 60;
    printf("%02d:%02d:%02d\r",
           hrs,
           min % 60,
           sec % 60);
}
#endif

int clock_free_processes();


void handle_mtimer_interrupt() {
    // trigger a supervisor timer interrupt
    csr_set(mip, MIP_STIP);
    // disable machine timer interrupt (or otherwise we would go back here right
    // after the ret instruction)
    csr_clear(mie, MIP_MTIP);
}


void handle_stimer_interrupt() {
    const uint64_t delta = 1000 / CLK_IT_FREQ; // ms

    timer++;

    /* Configuration de la prochaine interruption timer machine :
     * - configuration de mtimecmp 10 delta dans le future;
     * - va automatiquement acquitter MTIP;
     * - réactivation des interruptions timer machine.
     * - acquittement de l'interruption timer supervisor car le bit
     *   correspondant dans sip est read-only en mode superviseur
     */
    sbi_call_set_timer(delta);

#ifdef PRINT_TIME
    if (timer % 100 == 0) // 1 seconde
       print_time();
#endif

    // unblock if it's time or if a process is freed
    if (timer % (CLK_IT_FREQ / SCHED_FREQ) == 0 || clock_free_processes()) {
        schedule();
    }
}

// Endors le processus tant que "clock" n'est pas passée.
void wait_clock(unsigned long clock) {
    sched_block(clock, &blocked_processes, BLOCKED_ON_CLOCK, NULL);
}

/* 
 * Réveille les processus en attente sur l'horloge
 * et renvoie un entier indiquant si un processus 
 * plus prioritaire nécessite de scheduler.
 */
int clock_free_processes() {
    int scheduling = 0;

    /* Débloquer tous les processus qui le doivent */
    process_t *p = PROCESS_QUEUE_BOTTOM(&blocked_processes);
    while (p != NULL && (unsigned int) p->info <= timer) {
        scheduling = sched_unblock(p) || scheduling;
        p = PROCESS_QUEUE_BOTTOM(&blocked_processes);
    }
    /* Retourner l'information sur le scheduling */
    return scheduling;
}

/* Attend x secondes
 * <sec>: nombre de secondes à attendre
 */
void sleep(unsigned long sec) {
    wait_clock(timer + sec * CLK_IT_FREQ);
}

/* Attend x milli-secondes
 * <ms>: nombre de ms à attendre
 * Attention, ne pas descendre en dessous de 10ms
 */
void sleepms(unsigned long ms) {
    wait_clock(timer + (ms * CLK_IT_FREQ) / 1000);
}

/* execute <callback> en boucle pendant <sec> secondes
 * <sec>: nombre de secondes à attendre
 */
void do_for_seconds(int sec, void (*callback)()) {
    unsigned int timeout = timer + sec * CLK_IT_FREQ;
    while (timer < timeout)
        callback();
}

// initialise l'horloge
void clock_init() {
    // On programme la première interruption machine timer
    const uint32_t delta = 1000 / CLK_IT_FREQ; // ms
    /* Appel SBI set timer :
     * - Configuration du registre mtimecmp dans le future
     * - Activation des interruptions timer machine
     */
    sbi_call_set_timer(delta);

    // Activation des interruption timer supervisor
    csr_set(sie, MIP_STIP);
}

void clock_settings(unsigned long *quartz, unsigned long *ticks) {
    *quartz = clint_dev->clk_freq;
    *ticks = clint_dev->clk_freq / CLK_IT_FREQ;
}

unsigned long current_clock(void) {
    return timer;
}
