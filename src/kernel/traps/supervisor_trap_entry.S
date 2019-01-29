
.globl strap_entry

	.text
	.align 2
strap_entry:
    # On regarde de quel monde on vient
    csrw sscratch, t0
    csrr t0, sstatus
    andi t0, t0, 0x100
    bnez t0, switch_stack_from_kernel
switch_stack_from_user:
    # On récupère t0, et on sauvegarde sp_user
    csrrw t0, sscratch, sp
    la sp, sav_stack_kernel
    ld sp, (sp)
    j push_context
switch_stack_from_kernel:
    csrrw t0, sscratch, sp
    # On alligne sur 8 octets
    srli sp, sp, 3
    slli sp, sp, 3
push_context:
    # Sauvegarde du contexte
    addi	sp, sp, -136
    sd ra, 0(sp)
    sd a0, 8(sp)
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
    # On regarde si on vient du mond user ou kernel
    # on enregistre l'information sur la pile pour repartir dans le bon monde
    # SPP=0 -> from user space
    # SPP=1 -> from kernel space
	csrr t0, sstatus
	andi t1, t0, 0x100
	sd t1, 16*8(sp)

    # On récupère la cause et on regarde si c'est un syscall
    csrr t0, scause
    li t1, 8 # user_ecall
    beq t0, t1, syscall_entry
    mv a0, t0
    csrr	a1, sepc
    jal		strap_handler
    # Comme on est pas dans un syscall, on peut restaurer a0 (retval)
    ld		a0, 8(sp)



restore_context:
# On regarde si on vient du monde kernel ou user (info push sur la pile)
    ld t0, 16*8(sp)
    li t1, 0x100
    beqz t0, to_user_space
to_kernel_space:
    csrs sstatus, t1
    j restor_regs
to_user_space:
    csrc sstatus, t1
restor_regs:
la t0, sav_stack_kernel
addi t1, sp, 136
sd t1, (t0)
    ld ra, 0(sp)
    # Skip A0 : On restore A0 seulement dans le cas d'une interruption.
    ld a1, 8*2(sp)
	ld a2, 8*3(sp)
	ld a3, 8*4(sp)
	ld a4, 8*5(sp)
	ld a5, 8*6(sp)
	ld a6, 8*7(sp)
	ld a7, 8*8(sp)
	ld t0, 8*9(sp)
	ld t1, 8*10(sp)
	ld t2, 8*11(sp)
	ld t3, 8*12(sp)
	ld t4, 8*13(sp)
	ld t5, 8*14(sp)
	ld t6, 8*15(sp)
	addi sp, sp, 136
	csrrw sp, sscratch, sp
	sret


syscall_entry:
    la t0, sysc_handlers
    slli t1, a5, 3
    add t1, t1, t0
    ld t1, (t1)
    jalr ra, 0(t1)
    # SEPC is into a7
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    j restore_context
