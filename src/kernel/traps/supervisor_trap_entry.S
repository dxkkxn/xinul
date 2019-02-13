
#include "encoding.h"

.macro save_trap_frame
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
    sd s0, 16*8(sp)
    sd s1, 17*8(sp)
    sd s2, 18*8(sp)
    sd s3, 19*8(sp)
    sd s4, 20*8(sp)
    sd s5, 21*8(sp)
    sd s6, 22*8(sp)
    sd s7, 23*8(sp)
    sd s8, 24*8(sp)
    sd s9, 25*8(sp)
    sd s10, 26*8(sp)
    sd s11, 27*8(sp)
    sd tp, 28*8(sp)
    sd gp, 29*8(sp)

    # On récupère sp, epc, badvaddr, cause
    # sp est dans sscratch
    csrrw t0, sscratch,x0
    sd t0, 30*8(sp)
    csrr s0, sstatus
    sd s0, 31*8(sp)
    csrr s1, sepc
    sd s1, 32*8(sp)
    csrr t1, stval
    sd t1, 33*8(sp)
    csrr t2, scause
    sd t2, 34*8(sp)

  # get faulting insn, if it wasn't a fetch-related trap
    li t0, -1
    sd t0, 35*8(sp)
1:
    .endm


    .text
    .align 2
.globl strap_entry
strap_entry:
    # On récupère la pile kernel sauvgardée dans sscratch
    csrrw sp, sscratch, sp
    # Si elle est null, c'est que l'on a déjà la pile kernel
    bnez sp, 1f
    csrr sp, sscratch
1:
    # Sauvegarde du contexte
    addi sp, sp, -36*8
    save_trap_frame
    # On récupère la cause et on regarde si c'est un syscall
    csrr t0, scause
    li t1, 8 # user_ecall
    beq t0, t1, syscall_entry
    mv a0, sp
    jal strap_handler
    # Comme on est pas dans un syscall, on peut restaurer a0 (retval)
    ld a0, 8(sp)


restore_context:
    # On enregistre sp kernel dans sscratch dans le cas d'un processus user
    # On regarde si on vient du monde kernel ou user (sstatus est dans S0)
    andi s0, s0, SSTATUS_SPP
    bnez s0, 1f
    addi t0, sp, 36*8
    csrw sscratch, t0

1:
    csrw sepc, s1
    ld t1, 31*8(sp)
    csrw sstatus, t1
    # restore_regs:
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
    ld s0, 16*8(sp)
    ld s1, 17*8(sp)
    ld s2, 18*8(sp)
    ld s3, 19*8(sp)
    ld s4, 20*8(sp)
    ld s5, 21*8(sp)
    ld s6, 22*8(sp)
    ld s7, 23*8(sp)
    ld s8, 24*8(sp)
    ld s9, 25*8(sp)
    ld s10, 26*8(sp)
    ld s11, 27*8(sp)
    ld tp, 28*8(sp)
    ld gp, 29*8(sp)
    ld sp, 30*8(sp)
    sret


syscall_entry:
    la t0, sysc_handlers
    slli t1, a5, 3
    add t1, t1, t0
    ld t1, (t1)
    jalr ra, 0(t1)
    # SEPC is into s1
    addi s1, s1, 4
    j restore_context
