/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "assert.h"

#include "riscv.h"
#include "csr.h"
#include "sbi.h"
#include "device.h"

/*
 * sbi_call_set_timer
 *
 * Cette fonction configure la prochaine interruption timer machine delta ms dans le future.
 *
 * Cette fonction traite l'appel sbi set timer
 *
 *@param delta : réglage de la prochaine interruption à cur + delta ms.
 */
void handle_sbi_set_timer(uint64_t delta)
{
	set_mtimecmp(get_mtime() + delta * (clint_dev->clk_freq / 1000));
	csr_set(mie, MIP_MTIP);
}

uint64_t handle_sbi_call(
		uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
	csr_write(mepc, csr_read(mepc) + 4);
	switch (call_no) {
		case SBI_SET_TIMER:
			handle_sbi_set_timer(arg0);
			break;
		default:
			die("machine mode: sbi call %ld\n", call_no);
			break;
	}

	return 0;
}
