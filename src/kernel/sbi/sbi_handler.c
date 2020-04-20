/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019 - 2020
 *
 * See license for license details.
 */

#include "assert.h"
#include "riscv.h"

#include "sbi/sbi.h"
#include "timer.h"

/*
 * SBI handlers
 *
 * Les fonctions de ce fichier sont appelées suite à un appel SBI (passage du mode supervisor au mode machine).
 * Ces fonctions doivent donc être exécutées en mode machine.
 */

/*
 * sbi handler set_timer
 *
 * Cette appel sbi fournit  un service de timer pour le mode supervisor.
 * Celui-ci sera notifié delta ms dans le future par la levé d'une interruption timer supervisor (STI).
 *
 * Plus précisément, cette appel sbi depuis le mode supervisor a pour but de:
 * - aquitter l'interruption timer supervisor (opération seulement possible en mode machine) (bit STIP);
 * - programmer une interruption timer supervisor delta ms dans le futur par l'intermédiaire d'une interruption timer machine.
 *
 *@param delta : réglage de la prochaine interruption à cur + delta ms.
 */
void handle_sbi_set_timer(uint64_t delta)
{
#ifndef STUDENT
	set_machine_timer_interrupt(delta);
#endif // STUDENT

	/*
	 * Il faut acquiter l'interruption timer supervisor ici car SIP_STIP est read-only en mode superviseur
	 */
#ifndef STUDENT
	csr_clear(mip, MIP_STIP);
#endif // STUDENT
}

uint64_t handle_sbi_call(
		uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
	csr_write(mepc, csr_read(mepc) + 4);
	switch (call_no)
	{
		case SBI_SET_TIMER:
			handle_sbi_set_timer(arg0);
			break;
		default:
			die("machine mode: sbi call %ld\n", call_no);
			break;
	}
	
	return 0;
}
