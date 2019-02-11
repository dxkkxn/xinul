#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"


int main(void * arg)
{
	char buf[10];
	int dur = 5;
	volatile int * msg = shm_acquire("demo");

	printf("Bob : Bonjour, je vais attendre que ma soeure Anne me donne son nombre\n");
	while(*msg == 0) {
		printf("Bob : J'attend Anne encore %d secondes\n", dur);
		sleep(dur);
	}
	printf("Bob : J'ai eu le message de Anne qui m'a envoyée %d comme nombre\n", *msg);
	printf("Bob : Combien voulez vous que j'ajoute à ce nombre : ");
	cons_read(buf, 5);
	int somme = strtol(buf, NULL,10);
	int resultat = *msg + somme;
	printf("Bob je fais donc %d + %d = %d et je le dis à mon père par valeur de retour\n", *msg, somme, resultat);

	return resultat;

}