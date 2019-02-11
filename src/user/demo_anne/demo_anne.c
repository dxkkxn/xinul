#include "stdio.h"
#include "syscall.h"


int main(void)
{
	int64_t nombre;
	puts("Programme de démonstration OSON sur RISC-V");
	puts("Je vais créer deux fils Anne et Bob, et je vais attendre qu'il me donne un nombre");
	int pid = start("demo_anne", 4000, 128, NULL);
	printf("Ma fille anne a été créée avec le pid %d\n", pid);
	int pid2 = start("demo_bob", 4000, 128, NULL);
	waitpid(pid2, &nombre);
	printf("Bob m'a répondu %ld\n", nombre);


	return 0;

}