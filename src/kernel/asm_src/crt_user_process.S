# See LICENSE for license details.

.text
.globl crt_user_process
crt_user_process:
# Quand le programme se termine on doit appeler sched_exit() pour libérer correctement le processus.
# L'adresse de la fonction a été provisoirement placée dans s0.
	mv ra, s0
	# On charge le paramètre arg du programme provisoirement placé dans s1.
	mv a0, s1
	# set previous mode to user (SPP=0)
	# Note : in user-mode, SIE is always enabled.
		li t0, 0x100
		csrc sstatus, t0
		sret

