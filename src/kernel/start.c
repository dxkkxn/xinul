#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "process.h"
#include "program.h"

#include "interrupts.h"
#include "virtual_memory.h"


void ctx_sw(struct context *, struct context *);

void idle()
{
	printf("Je suis idle\n");
	process_t idle = get_process(0);
	process_t hello = get_process(1);
	process_t hello_user = get_process(2);
	ctx_sw(&idle->context, &hello->context);
	printf("Encore une fois dans le idle\n");
	ctx_sw(&idle->context, &hello->context);
	printf("on refait un tour dans idle et on lance hello_user\n");
	ctx_sw(&idle->context, &hello_user->context);
	
	printf("On entre dans la boucle infinie du idle\n");
	unsigned int i = 0;
	while(i < 10) {
		printf("timer interrupt n°%d\n", i);
		__asm__("wfi");
		i++;
	}
	printf("On sort de la boucle infinie pour éviter de faire un make kill\n");
}
extern char _hello_start[];
extern char _hello_end[];

int main()
{
	printf("\n= OSON Initialization =\n");
	
	printf("hello start %p\n", _hello_start);
	printf("hello end %p\n", _hello_end);
	int64_t size = (int64_t)(_hello_end) - (uint64_t)(_hello_start);
	printf("hello size %d\n", (int)size);
	exit(0);
	

	setup_clock_interrupts();
	init_process();
	
	init_virtual_memory();


	if ( (create_kernel_process(hello, "Hello", 100, (void*) 42)) == NULL)
	{
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	
	process_t hello_user_process = create_user_process("hello_user", "Hello_user", 100, 0, (void*) 42);
	if (hello_user_process == NULL)
		{
			printf("Process error: unable to create user process hello_user. exit.\n");
			exit(-1);
		}
		
	idle();

	return 0;
}

