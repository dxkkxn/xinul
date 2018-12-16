#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "scheduler.h"
#include "program.h"

#include "interrupts.h"
#include "virtual_memory.h"


int main()
{
	printf("\n= OSON Initialization =\n");

	printf("Clock interruptions...");
	setup_clock_interrupts();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	//init_virtual_memory();


	if (sched_kstart(systemd, 1, "systemd", (void *) 0) < 0) {
		assert(0 && "Unable to create systemd process");
	}
	return 0;
}
