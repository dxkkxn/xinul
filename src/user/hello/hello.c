#include "stdio.h"

#include "syscall.h"
#include "stdlib.h"

int main(void)
{
	printf("Je suis l'app hello et je suis dans l'espace user\n");
	void *obj = malloc(50);
	printf("obj allocation address %p\n", obj);
	printf("On rentre dans une boucle\n");
	uint64_t i=0;
	while(i++<1<<28);
	printf("Fin du programme hello, retval = 42\n");
	exit(42);
//	printf("Hellowords from User space.\n");
	return 43;
}
