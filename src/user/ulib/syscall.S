/*
 * Mathieu Barbe <mathieu@kolabnow.com> 2019
 * Ensimag, Projet Système 2018
 *
 * Stubs vers le noyau de la bibliothéque d'appels systèmes.
 */

#include "syscall_num.h"

/*
 * Appels systèmes avec 0 à 5 paramètres
 */
#define SYSCALL(name)            \
.globl name;                 \
name:                        \
    li    a5, SYSC_##name;   \
    ecall;                   \
    ret

SYSCALL(chprio)
SYSCALL(clock_settings)
SYSCALL(cons_echo)
SYSCALL(cons_chbuffer)
SYSCALL(cons_read)
SYSCALL(cons_write)
SYSCALL(cons_wait)
SYSCALL(current_clock)
SYSCALL(exit)
SYSCALL(getname)
SYSCALL(getpid)
SYSCALL(getprio)
SYSCALL(getstatus)
SYSCALL(kill)
SYSCALL(pcount)
SYSCALL(pcreate)
SYSCALL(pdelete)
SYSCALL(preceive)
SYSCALL(preset)
SYSCALL(psend)
SYSCALL(psize)
SYSCALL(sleep)
SYSCALL(start)
SYSCALL(wait_clock)
SYSCALL(waitpid)
SYSCALL(waitpid_nohang)
SYSCALL(sleepms)
SYSCALL(shm_create)
SYSCALL(shm_acquire)
SYSCALL(shm_release)

.globl sys_info
sys_info:
    ret
