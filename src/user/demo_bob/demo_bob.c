#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"


int main(void * arg)
{
	int64_t nombre = (int64_t) arg;
	char buf[10];
	printf("Je suis Anne et mon père m'a donné le chiffre %ld\nPar combien voulez-vous que je le multiplie?\n", nombre);
	cons_read(buf, 5);
	int facteur = strtol(buf, NULL,10);
	int resultat = nombre * facteur;
	printf("je fais donc %ld x %d = %d et je le dis à mon frère par mémoire partagée\n", nombre, facteur, resultat);


	puts("Programme de démonstration OSON sur RISC-V");
	puts("Je vais créer deux fils Anne et Bob, et je vais attendre qu'il me donne un nombre");
	int pid = start("demo_anne", 4000, 128, NULL);
	printf("Ma fille anne a été créée avec le pid %d\n", pid);
	int pid2 = start("demo_bob", 4000, 128, NULL);
	waitpid(pid2, &nombre);
	printf("Bob m'a répondu %ld\n", nombre);


	return 0;

}