

#include "stdio.h"
#include "assert.h"
#include "stdint.h"
#include "riscv.h"

#include "scheduler.h"
#include "program.h"
#include "tests/tests.h"
#include "sbi/sbi.h"

int64_t hello_user(void* arg)
{
printf("test sys call\n");
SBI_CALL_3(3, 4, 5, 6);
printf("Programme user hello\nTentative de lire un registre interdit\n");
	csr_read(sstatus);
	printf("erreur, lecture possible\n");
	
	return 0;
}

int64_t hello(void* arg)
{
	printf("Hello words! My pid is %d and my arg is %ld\n", sched_get_active_pid(), (uint64_t) arg);
	for (size_t i = 0; i < 100000000; i++) {

	}
	printf("Je suis à la fin du programme hello pid: %d\n", sched_get_active_pid());
	return 0;
}

int64_t autotest(void* arg)
{
	int pid;

	printf("[autotest] Starting...\n");
	
	pid = sched_kstart(test0, 128, "test0", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test1, 128, "test1", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test2, 128, "test2", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test3, 128, "test3", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test4, 128, "test4", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test5, 128, "test5", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test10, 128, "test10", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);
	
	printf("[autotest] Done\n");
	return 0;
}

int64_t systemd(void* arg)
{
	uint64_t pid;
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
	pid = sched_kstart(autotest, 2, "autotest", (void*) 0);
	assert(pid > 0);
	sched_waitpid(pid, NULL);

	printf("End of systemd\n");

	return 0;
}
