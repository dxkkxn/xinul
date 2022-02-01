#include "riscv.h"
#include "stdio.h"

#include "drivers/clint.h"
#include "timer.h"

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

}

void handle_mtimer_interrupt()
{
	printf("Tic ");

}

void handle_stimer_interrupt()
{
}

