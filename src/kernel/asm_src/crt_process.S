# See LICENSE for license details.

#include "encoding.h"

.text
.align 2
.globl crt_process
crt_process:
    # Quand le programme se termine on doit appeler sched_exit() pour libéterminer correctement le processus.
    # L'adresse a été provisoirement placée dans s0.
    mv ra, s0

    # On charge le paramètre arg du programme provisoirement placé dans s1.
    mv a0, s1

    # On indique qu'on travail seulement avec une pile kernel.
    csrw sscratch, zero

    # On indique que le mode à utiliser lors du sret est le mode superviseur (SPP=1)
    li t0, SSTATUS_SPP
    csrs sstatus, t0

    sret

