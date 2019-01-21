#include "stdio.h"

#include "syscall.h"

int main(void)
{
	printf("Je suis l'app hello et je suis dans l'espace user\n");
	exit(42);
//	printf("Hellowords from User space.\n");
	return 43;
}
