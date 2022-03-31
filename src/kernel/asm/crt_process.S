# See LICENSE for license details.

#include "encoding.h"

.text
.align 2
.globl crt_process
crt_process:
    # Quand le programme se termine on doit appeler sched_exit() pour terminer correctement le processus.
    # L'adresse de sched_exit a été provisoirement placée dans s0.
    mv ra, s0
    # On charge le paramètre arg du programme provisoirement placé dans s1.
    mv a0, s1
    sret
