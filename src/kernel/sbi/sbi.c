/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "sbi/sbi.h"

/*
 * sbi_call_set_timer
 *
 * Cette fonction configure la prochaine interruption timer machine delta ms dans le future.
 *
 * Cette appel sbi fournit  un service de timer pour le mode supervisor.
 * Celui-ci sera notifié delta ms dans le future par la levé d'une interruption timer supervisor (STI).
 *
 * Plus précisément, cette appel sbi depuis le mode supervisor a pour but de:
 * - aquitter l'interruption timer supervisor (opération seulement possible en mode machine) (bit STIP);
 * - programmer une interruption timer supervisor delta ms dans le futur.
 *
 *@param delta : réglage de la prochaine interruption à cur + delta ms.
 */
void sbi_call_set_timer(uint64_t delta)
{
	SBI_CALL_1(SBI_SET_TIMER, delta);
}
