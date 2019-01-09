# See LICENSE for license details.

.text
.globl crt_user_process
crt_user_process:
# Quand le programme se termine on doit appeler sched_user_exit() pour libéterminer correctement le processus.
# L'adresse a été provisoirement placée dans s0.
	mv ra, s0
	# On charge le paramètre arg du programme provisoirement placé dans s1.
	mv a0, s1
	# set previous mode to user (SPP=0) and with interrupt (SPIE=1)
	# in user-mode, the value in SIE is ignored, 
	# and supervisor-level interrupts are enabled. 
		li t0, 0x020
		csrs sstatus, t0
		# clear SPP
		li t0, 0x100
		csrC sstatus, t0
		sret

