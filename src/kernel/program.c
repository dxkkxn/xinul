

#include "stdio.h"

#include "process.h"
#include "program.h"

int hello_user(int arg)
{
	printf("Programme user hello\n");
	return 0;
}

int hello(int arg)
{
//process_t idle = get_process(0);
//process_t hello = get_process(1);
	printf("Hello words! My pid is %d and my arg is %d\n", getpid(), arg);
	for (size_t i = 0; i < 100000; i++) {

	}
	printf("Je suis à la fin du programme hello pid: %d\n", getpid());
	return 0;
}
