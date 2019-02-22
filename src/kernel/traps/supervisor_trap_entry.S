
#include "encoding.h"

.text
    .align 4
.globl strap_entry
strap_entry:
    # On récupère la pile kernel sauvgardée dans sscratch
    csrrw sp, sscratch, sp
    # Si elle est null, c'est que l'on a déjà la pile kernel
    bnez sp, 1f
    csrrw sp, sscratch, sp
1:
    # Sauvegarde du contexte
    addi sp, sp, -16*8
    sd ra, 0*8(sp)
    sd a0, 1*8(sp)
    sd a1, 2*8(sp)
    sd a2, 3*8(sp)
    sd a3, 4*8(sp)
sd a4, 5*8(sp)
    sd a5, 6*8(sp)
    sd a6, 7*8(sp)
    sd a7, 8*8(sp)
    sd t0, 9*8(sp)
    sd t1, 10*8(sp)
    sd t2, 11*8(sp)
    sd t3, 12*8(sp)
    sd t4, 13*8(sp)
    sd t5, 14*8(sp)
    sd t6, 15*8(sp)

    # On récupère la cause et on regarde si c'est un syscall
    csrr t0, scause
    li t1, 8 # user_ecall
    beq t0, t1, syscall_entry
    mv a0, t0
    csrr a1, sepc
    mv a2, sp
    jal strap_handler
    # Comme on ne traite pas un syscall, on doit restaurer a0 (retval)
    ld a0, 8(sp)


restore_context:
    ld ra, 0(sp)
    # Skip A0 : On restore A0 seulement dans le cas d'une interruption.
    ld a1, 2*8(sp)
    ld a2, 3*8(sp)
    ld a3, 4*8(sp)
    ld a4, 5*8(sp)
    ld a5, 6*8(sp)
    ld a6, 7*8(sp)
    ld a7, 8*8(sp)
    ld t0, 9*8(sp)
    ld t1, 10*8(sp)
    ld t2, 11*8(sp)
    ld t3, 12*8(sp)
    ld t4, 13*8(sp)
    ld t5, 14*8(sp)
    ld t6, 15*8(sp)
    addi sp, sp, 16*8
    # On sauvegarde la pile kernel dans sscratch
    csrrw sp, sscratch, sp
    # Si la pile user est null on est dans un processus kernel, on récupère la pile kernel
    bnez sp, 1f
    csrrw sp, sscratch, sp
1:
    sret


syscall_entry:
    la t0, sysc_handlers
    slli t1, a5, 3
    add t1, t1, t0
    ld t1, (t1)
    jalr ra, 0(t1)
    # on incrémente sepc pour passer à l'instruction suivante
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    j restore_context
