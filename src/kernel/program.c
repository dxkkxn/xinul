

#include "stdio.h"

#include "process.h"
#include "program.h"

void extern ctx_sw(struct cpu_state *, struct cpu_state *);

int hello(int arg)
{
process_t idle = get_process(0);
process_t hello = get_process(1);
	printf("Hello words! My pid is %d and my arg is %d\n", getpid(), arg);
	ctx_sw(&hello->cpu_state, &idle->cpu_state);
	printf("Je suis à la fin du programme hello\n");
	return 0;
}
