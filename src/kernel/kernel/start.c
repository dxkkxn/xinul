

#include "stdio.h"

#include "process.h"
#include "program.h"
#include "clock.h"

void idle()
{
	printf("Je suis idle\n");
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

	// Malloc does not work.
	//create_kernel_process(hello, "Hello", 100, 0);
	idle();

	return 0;
}
