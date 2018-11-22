

#include "stdio.h"

#include "process.h"
#include "program.h"
#include "clock.h"

void extern ctx_sw(struct cpu_state *, struct cpu_state *);


void idle()
{
	printf("Je suis idle\n");
	process_t idle = get_process(0);
	process_t hello = get_process(1);
	ctx_sw(&idle->cpu_state, &hello->cpu_state);
	printf("Encore une fois dans le idle\n");
	ctx_sw(&idle->cpu_state, &hello->cpu_state);
	printf("On entre dans la boucle infinie du idle\n");
	while(1);
}

int main(int argc, char **argv)
{
	printf("OSON Initialization.\n");
	init_process();
	init_machine_clock();
	
	unsigned int reg, i;

	i=0;
	while(1) {
			printf("timer interrupt n°%d\n\n", i);
			__asm__("wfi");
			i++;
	}

	create_kernel_process(hello, "Hello", 100, 42);
	idle();

	return 0;
}
