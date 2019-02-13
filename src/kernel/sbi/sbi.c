/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "sbi.h"

/*
 * sbi_call_set_timer
 *
 * Cette fonction configure la prochaine interruption timer machine delta ms dans le future.
 *
 * Cette appel sbi va :
 * - activer les interruption timer machine;
 * - configurer les registre du clint.
 *
 *@param delta : réglage de la prochaine interruption à cur + delta ms.
 */
void sbi_call_set_timer(uint64_t delta)
{
	SBI_CALL_1(SBI_SET_TIMER, delta);
}
