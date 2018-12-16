

#include "stdio.h"
#include "stdint.h"

#include "scheduler.h"
#include "program.h"
#include "tests.h"

int hello_user(void* arg)
{
	printf("Programme user hello\n");
	return 0;
}

int hello(void* arg)
{
	printf("Hello words! My pid is %d and my arg is %ld\n", sched_get_active_pid(), (uint64_t) arg);
	for (size_t i = 0; i < 100000000; i++) {

	}
	printf("Je suis à la fin du programme hello pid: %d\n", sched_get_active_pid());
	return 0;
}

int autotest(void* arg)
{
	int pid;

	printf("[autotest] Starting...\n");
	
	pid = sched_kstart(test1, 128, "test1", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	
	printf("[autotest] Done\n");
	return 0;
}

int systemd(void* arg)
{
	printf("Kernel systemd: when this process dies the OS is idle forever\n");

	/*
	if ((sched_kstart(hello, 100, "Hello", (void*) 42)) < 0) {
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	if ((sched_kstart(hello, 100, "Hello", (void*) 12)) < 0) {
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	*/
	if ((sched_kstart(autotest, 2, "autotest", (void*) 0)) < 0) {
		printf("Process error: unable to create process autotest. exit.\n");
		exit(-1);
	}

	printf("End of systemd, catching a few interrupts before sleeping\n");

	unsigned int i = 0;
	while (i < 10) {
		printf("Waiting interrupt %d...\n", i);
		__asm__("wfi");
		printf("Return from interrupt %d\n", i);
		i++;
	}
	return 0;
}
