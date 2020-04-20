/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#pragma once

#include "stdint.h"

// SBI numbers
#define SBI_SET_TIMER 0

uint64_t handle_sbi_call(uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2);

#define SBI_CALL(call_no, arg0, arg1, arg2) ({                    \
    register uintptr_t a0 __asm__ ("a0") = (uintptr_t)(arg0);    \
    register uintptr_t a1 __asm__ ("a1") = (uintptr_t)(arg1);    \
    register uintptr_t a2 __asm__ ("a2") = (uintptr_t)(arg2);    \
    register uintptr_t a7 __asm__ ("a7") = (uintptr_t)(call_no);    \
    __asm__ __volatile__ ("ecall"                                \
            : "+r" (a0)                                            \
            : "r" (a1), "r" (a2), "r" (a7)                        \
            : "memory");                                        \
    a0;                                                            \
})

#define SBI_CALL_0(call_no) SBI_CALL(call_no, 0, 0, 0)
#define SBI_CALL_1(call_no, arg0) SBI_CALL(call_no, arg0, 0, 0)
#define SBI_CALL_2(call_no, arg0, arg1) SBI_CALL(call_no, arg0, arg1, 0)
#define SBI_CALL_3(call_no, arg0, arg1, arg2) \
    SBI_CALL(call_no, arg0, arg1, arg2)


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
void sbi_call_set_timer(uint64_t delta);

