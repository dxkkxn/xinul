#include "stdio.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"


int main(void *arg)
{
	int64_t nombre = (int64_t) arg;
	char buf[10];


	printf("Anne : Salut, je suis Anne et mon père m'a donné le chiffre %ld\nPar combien voulez-vous que je le multiplie?\n",
		   nombre);
	cons_read(buf, 5);
	int facteur = strtol(buf, NULL, 10);
	int resultat = nombre * facteur;
	printf("je fais donc %ld x %d = %d et je le dis à mon frère par mémoire partagée\n", nombre, facteur, resultat);
	volatile int *msg = shm_acquire("demo");
assert(msg != 0);
	*msg = resultat;
	puts("Anne : fin");
	return 0;

}