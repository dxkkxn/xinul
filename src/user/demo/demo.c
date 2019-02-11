#include "stdio.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"



int main(void)
{
	int64_t nombre;
	int prio_alice = 100;
	int prio_bob = 101;
	char buf[6];
	puts("# Programme de démonstration OSON sur RISC-V #\n");

	void* msg = shm_create("demo");
	assert(msg != 0);

	printf("[Père] Je suis le pid %d et ma priorité est %d\n", getpid(), getprio(getpid()));
	puts("[Père] Je vais créer deux fils Alice et Bob, et je vais attendre qu'il me donne un nombre à partir d'une graine");
	printf("[Père] Donnez moi une graine : ");
	cons_read(buf, 5);
	int64_t graine = strtol(buf, NULL, 10);

	int pid = start("demo_alice", 4000, prio_alice, (void*)graine);
	printf("[Père] Ma fille Alice a été créée avec la graine %ld en argument et pid=%d, priorité=%d\n", graine, pid, getprio(pid));

	int pid2 = start("demo_bob", 4000, prio_bob, NULL);
	printf("[Père] Mon fils Bob a été créée avec pid=%d et priorité=%d\n", pid2, getprio(pid2));

	puts("[Père] Je vais attendre que Bob est terminé et me donne sa réponse\n");
	waitpid(pid2, &nombre);
	printf("[Père] Bob m'a retourné son nombre %ld\n", nombre);
	printf("[Père] Fin\n");


	return 0;

}
