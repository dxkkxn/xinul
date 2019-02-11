#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"


int main(void * arg)
{
	char buf[10];
	int dur = 5;
	volatile int * msg = shm_acquire("demo");

	printf("\t\t[Bob] Bonjour, je vais attendre que ma soeur Alice me donne son nombre\n");
	while(*msg == 0) {
		printf("\t\t[Bob] J'attends Alice encore %d secondes\n", dur);
		sleep(dur);
	}
	printf("\t\t[Bob] J'ai eu le message d'Alice qui m'a envoyée %d comme nombre\n", *msg);
	printf("\t\t[Bob] Combien voulez vous que j'ajoute à ce nombre : ");
	cons_read(buf, 5);
	int somme = strtol(buf, NULL,10);
	int resultat = *msg + somme;
	printf("\t\t[Bob] Je fais donc %d + %d = %d et je le dis à mon père par valeur de retour\n", *msg, somme, resultat);
	printf("\t\t[Bob] Fin\n");

	return resultat;

}
