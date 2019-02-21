.text
.globl ctx_sw
.align 2
ctx_sw:
	# sauvegarde du contexte de l'ancien processus
	sd sp, 0(a0)
	sd ra, 8(a0)
	sd s0, 16(a0)
	sd s1, 24(a0)
	sd s2, 32(a0)
	sd s3, 40(a0)
	sd s4, 48(a0)
	sd s5, 56(a0)
	sd s6, 64(a0)
	sd s7, 72(a0)
	sd s8, 80(a0)
	sd s9, 88(a0)
	sd s10, 96(a0)
	sd s11, 104(a0)
	csrr t0, satp
	sd t0, 112(a0)
	csrr t1, sepc
	sd t1, 120(a0)
	# Sauvegarde du SP user dans sscratch
	csrr t2, sscratch
	sd t2, 128(a0)
		csrr t3, sstatus
    	sd t3, 136(a0)

	
	# Restauration du contexte
	ld sp, 0(a1)
	ld ra, 8(a1)
	ld s0, 16(a1)
	ld s1, 24(a1)
	ld s2, 32(a1)
	ld s3, 40(a1)
	ld s4, 48(a1)
	ld s5, 56(a1)
	ld s6, 64(a1)
	ld s7, 72(a1)
	ld s8, 80(a1)
	ld s9, 88(a1)
	ld s10, 96(a1)
	ld s11, 104(a1)
	ld t0, 112(a1)
	csrw satp, t0
	ld t1, 120(a1)
	csrw sepc, t1
	# restoration du SP user dans sscratch
	ld t2, 128(a1)
	csrw sscratch, t2
ld t3, 136(a1)
	csrw sstatus, t3

	# on passe la main au nouveau processus
	ret

