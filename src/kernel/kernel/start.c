

#include "stdio.h"

#include "process.h"
#include "program.h"

void idle()
{
	printf("Je suis idle\n");
	while(1);
}

int main(int argc, char **argv)
{
	printf("OSON Initialization.\n");
	
	init_process();
	
	// Malloc does not work.
	//create_kernel_process(hello, "Hello", 100, 0);
	idle();
	return 0;
}
