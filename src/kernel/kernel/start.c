#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

#include "process.h"
#include "program.h"
#include "clock.h"

#include "csr.h"
#include "stdint.h"


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
	unsigned int reg, i;
	i=0;
	while(i < 10) {
		printf("timer interrupt n°%d\n", i);
		__asm__("wfi");
		i++;
	}
	printf("On sort de la boucle infinie pour éviter de faire un make kill\n");
}



int main(int argc, char **argv)
{
	printf("\n= OSON Initialization =\n");
	init_process();
	//init_machine_clock();

	if ( (create_kernel_process(hello, "Hello", 100, (void*) 42)) == NULL)
	{
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	idle();

	return 0;
}
