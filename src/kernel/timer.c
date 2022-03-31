#include "riscv.h"
#include "stdio.h"

#include "drivers/clint.h"
#include "timer.h"
#ifndef STUDENT
#include "sbi/sbi.h"
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

#endif // STUDENT

/*
 * Set machine timer
 *
 * Cette fonction configure la prochaine interruption timer machine delta ms dans le futur.
 *
 *@param delta_ms : réglage de la prochaine interruption àtimer machine  cur + delta ms.
 */
void set_machine_timer_interrupt(uint64_t delta_ms)
{

	/*
	 * Configuration du clint
	 * L'adresse et la fréquence du clint est dépendante de la plateforme,
	 * La variable globale clint_dev de type clint_device_t nous renseigne sur ces paramettres:
	 * - clint_dev->clk_freq indique la fréquence d'incrémentations du registre time.
	 * - clint_dev->base_addr indique l'adresse où est mapé la base des registres du clint.
	 *
	 * Les macros get_mtime() et set_mtimecmp(x) de "drivers/clint.h" sont données pour lire et écrire les registres de ce composant.
	 */

#ifndef STUDENT
	set_mtimecmp(get_mtime() + delta_ms * (clint_dev->clk_freq / 1000));

	// Activation du timer machine
	csr_set(mie, MIP_MTIP);

#endif // STUDENT
}

void handle_mtimer_interrupt()
{
#ifdef STUDENT
	printf("Tic ");
#else // END STUDENT

	// trigger a supervisor timer interrupt
	csr_set(mip, MIP_STIP);

	/*
	 * disable machine timer interrupt (or otherwise we
	 * would go back here right after the ret instruction)
	 */
	csr_clear(mie, MIP_MTIP);
#endif // STUDENT

}

void handle_stimer_interrupt()
{
#ifndef STUDENT
	const uint64_t delta_ms = 1000 / CLK_IT_FREQ;

	timer++;

	/*
	 * Configuration de la prochaine interruption timer.
	 */
	if (clint_dev->supervisor_clint_available)
	{
	/*
	 * - configuration de stimecmp delta_ms dans le future;
	 * - va automatiquement acquitter STIP;
	 */
		set_stimecmp(get_stime() + delta_ms * (clint_dev->clk_freq / 1000));
	} else
	{
		/*
		 * Configuration du timer par SBI en mode machine
		 * L'acqittement de l'interruption timer supervisor doit être réalisée en mode machine
		 * car le bit STIP dans sip est read-only en mode superviseur.
		 */
		sbi_call_set_timer(delta_ms);
	}

#ifdef PRINT_TIME
	if (timer % 100 == 0) // 1 seconde
	   print_time();
#endif

	// unblock if it's time or if a process is freed
	if(timer % (CLK_IT_FREQ / SCHED_FREQ) == 0 || clock_free_processes())
	{
		schedule();
	}
#endif // STUDENT
}

#ifndef STUDENT
// Endors le processus tant que "clock" n'est pas passée.
void wait_clock(unsigned long clock)
{
	sched_block(clock, &blocked_processes, BLOCKED_ON_CLOCK, NULL);
}

/* 
 * Réveille les processus en attente sur l'horloge
 * et renvoie un entier indiquant si un processus 
 * plus prioritaire nécessite de scheduler.
 */
int clock_free_processes()
{
	int scheduling = 0;

	/* Débloquer tous les processus qui le doivent */
	process_t *p = PROCESS_QUEUE_BOTTOM(&blocked_processes);
	while (p != NULL && (unsigned int) p->info <= timer)
	{
		scheduling = sched_unblock(p) || scheduling;
		p = PROCESS_QUEUE_BOTTOM(&blocked_processes);
	}
	/* Retourner l'information sur le scheduling */
	return scheduling;
}

/* Attend x secondes
 * <sec>: nombre de secondes à attendre
 */
void sleep(unsigned long sec)
{
	wait_clock(timer + sec * CLK_IT_FREQ);
}

/* Attend x milli-secondes
 * <ms>: nombre de ms à attendre
 * Attention, ne pas descendre en dessous de 10ms
 */
void sleepms(unsigned long ms)
{
	wait_clock(timer + (ms * CLK_IT_FREQ) / 1000);
}

/* execute <callback> en boucle pendant <sec> secondes
 * <sec>: nombre de secondes à attendre
 */
void do_for_seconds(int sec, void (*callback)())
{
	unsigned int timeout = timer + sec * CLK_IT_FREQ;
	while (timer < timeout)
		callback();
}

// initialise l'horloge
void clock_init()
{
	// On programme la première interruption machine timer
	const uint32_t delta_ms = 1000 / CLK_IT_FREQ;
	if(clint_dev->supervisor_clint_available)
	{
		set_stimecmp(get_stime() + delta_ms * (clint_dev->clk_freq / 1000));
	} else
	{
		/* Appel SBI set timer :
		 * - Configuration du registre mtimecmp dans le future
		 * - Activation des interruptions timer machine
		 */
		sbi_call_set_timer(delta_ms);
	}

	// Activation des interruption timer supervisor
	csr_set(sie, MIP_STIP);
}

void clock_settings(unsigned long *quartz, unsigned long *ticks)
{
	*quartz = clint_dev->clk_freq;
	*ticks = clint_dev->clk_freq / CLK_IT_FREQ;
}

unsigned long current_clock(void)
{
	return timer;
}

#endif // STUDENT
