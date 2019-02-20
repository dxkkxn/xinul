# See LICENSE for license details.

#include "encoding.h"

.text
.globl crt_user_process
.align 2
crt_user_process:
    # Quand le programme se termine on doit appeler sched_exit() pour libérer correctement le processus.
    # L'adresse de la fonction a été provisoirement placée dans s0.
    mv ra, s0

    # On charge le paramètre arg du programme provisoirement placé dans s1.
    mv a0, s1

    # On enregistre la pile kernel placée dans s2
    csrw sscratch, s2

    # On indique que le mode à utiliser lors du sret est le mode user (SPP=0)
    # Note : les interruptions superviseur (bit SIE) sont toujours activées dans le mode user.
    li t0, SSTATUS_SPP
    csrc sstatus, t0

    sret

