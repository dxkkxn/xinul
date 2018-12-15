# See LICENSE for license details.

.text
.globl crt_process
crt_process:
# Quand le programme se termine on doit appeler process_exit() pour libéterminer correctement le processus.
# L'adresse a été provisoirement placée dans s0.
	mv ra, s0
	csrr t0, sepc
	nop
	# On charge le paramètre arg du programme provisoirement placé dans s1.
	mv a0, s1
	# Lancement du programme, l'adresse est placé provisoirement dans s3
	# set previous mode to superviseur (SPP=1) and with interrupt (SPIE=1)
	li t0, 0x120
	csrs sstatus, t0
	nop
	sret

