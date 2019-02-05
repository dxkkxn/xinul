# See LICENSE for license details.

#include "macros.h"
#include "constants.h"


.section .text.init,"ax",@progbits
.globl _start

_start:
    # setup default trap vector
    la      t0, mtrap_entry
    csrw    mtvec, t0

    # On place dans mscratch le haut de la pile machine
    # elle sera utilisé lors d'une interruption machine
    la t0, msp
    ld t0, (t0)
    csrw mscratch, t0

    # set up stack pointer based on hartid
    csrr    t0, mhartid
    slli    t0, t0, STACK_SHIFT
    la      sp, stacks + STACK_SIZE
    add     sp, sp, t0

    # park all harts excpet hart 0
    csrr    a0, mhartid
    bnez    a0, park

    j       boot_riscv

    # sleeping harts mtvec calls trap_fn upon receiving IPI
park:
    wfi
    j       park



    .bss
    .align 4
    .global stacks
stacks:
    .skip STACK_SIZE * MAX_HARTS
