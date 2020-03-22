/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "assert.h"

#include "encoding.h"
#include "riscv.h"

#include "sbi.h"
#include "device.h"
#include "timer.h"

/*
 * SBI handlers
 *
 * Les fonctions ce fichier sont appelées suite à un appel SBI.
 * Ces fonctions doivent donc être exécutées en mode machine.
 */

/*
 * sbi handler set_timer
 *
 * Cette fonction configure la prochaine interruption timer supervisor delta ms dans le future.
 *
 * Elle a pour but :
 * - aquitter l'interruption timer supervisor (opération seulement possible en mode machine);
 * - programmer une interruption timer supervisor delta ms dans le futur.
 *
 *@param delta : réglage de la prochaine interruption à cur + delta ms.
 */
void handle_sbi_set_timer(uint64_t delta)
{
	set_machine_timer_interrupt(delta);
	
	/*
	 * Il faut acquiter l'interruption ici car SIP_STIP est read-only en mode superviseur
	 */
	csr_clear(mip, MIP_STIP);
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
