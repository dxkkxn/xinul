#include "stdio.h"

#include "syscall.h"
#include "stdlib.h"

int main(void)
{
	printf("Je suis l'app hello et je suis dans l'espace user\n");
	void *obj = malloc(50);
	printf("obj allocation address %p\n", obj);
	exit(42);
//	printf("Hellowords from User space.\n");
	return 43;
}
