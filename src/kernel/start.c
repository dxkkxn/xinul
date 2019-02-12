#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "scheduler.h"
#include "program.h"
#include "tests.h"
#include "supervisor_trap.h"
#include "vmm.h"
#include "hmm.h"
#include "shm.h"
#include "mapper.h"
#include "syscall.h"
#include "timer.h"
#include "kbd.h"

char default_program[20];

int64_t launcher(void *arg)
{
	int pid;

	printf("Launcher start.\n");
	if ((pid = sched_ustart(default_program, 1024, 11, (void *) 0)) < 0) {
		assert(0 && "Unable to run app autotest\n");
	}
	sched_waitpid(pid, NULL);

#if 0
	if ((pid = sched_ustart("autotest", 1024, 11, (void *) 0)) < 0) {
		assert(0 && "Unable to run app autotest\n");
	}
	sched_waitpid(pid, NULL);

	printf("Nombre de frame disponible avant app hello : %d\n", hmm_frame_count());
	if ((pid = sched_ustart("hello", 1024, 11, (void *) 0)) < 0) {
		assert(0 && "Unable to create programme hello userde test ");
	}
	sched_waitpid(pid, NULL);
//	printf("Nombre de frame disponible aprèes app hello : %d\n", hmm_frame_count());
#endif
	return 0;
}

int64_t kernel_tests(void *arg)
{
	printf("Tests kernel\n");
	int pid = sched_kstart(test0, 10, "Test 0", 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test1, 10, "test1", 0);
	sched_waitpid(pid, NULL);

	puts("Kernel tests done.");
	return 0;
}

int kernel_start()
{
	printf("\n= OSON Initialization =\n");

	printf("Keyboard initialization...");
	kbd_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Hardware memory manager initialization...");
	extern char _free_memory_start[];
	extern char _memory_end[];
	extern char _bss_start[];
	extern char _bss_end[];
	(void) _bss_end;
	(void) _bss_start;
	hmm_init(_free_memory_start, _memory_end);
	printf("\r\t\t\t\t\t\t\t[OK]\n");
	printf("Number of frames %d = %d ko\n", hmm_frame_count(), hmm_free_memory() >> 10);

	printf("Clock interruptions...");
	clock_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Virtual memory manager initialization...");
	init_virtual_memory();

	if (is_virtual_memory_enable()) {
		printf("\r\t\t\t\t\t\t\t[OK]\n");

		printf("Shared memory API initialization...");
		shm_init();
		printf("\r\t\t\t\t\t\t\t[OK]\n");

		printf("Syscall initialization...");
		sysc_init();
		printf("\r\t\t\t\t\t\t\t[OK]\n");

		sched_kstart(launcher, 10, "Launcher", 0);

	} else {
		printf("\r\t\t\t\t\t\t\t[NOT AVAILABLE]\n");
		printf("# Virtual memory not available, start kernel tests\n");
		sched_kstart(kernel_tests, 10, "Launcher", 0);
	}

	assert(0 && "end of main");
}
