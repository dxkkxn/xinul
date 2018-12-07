# See LICENSE for license details.

.text
.globl crt_user_process
crt_user_process:
# Quand le programme se termine on doit appeler process_exit() pour libéterminer correctement le processus.
# L'adresse a été provisoirement placée dans s0.
	mv ra, s0
	# On charge le paramètre arg du programme provisoirement placé dans s1.
	mv a0, s1
	# Lancement du programme, l'adresse est placé provisoirement dans s3
	jalr x0, s2

