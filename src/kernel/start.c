/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "stdio.h"
#include "assert.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#include "drivers/splash.h"
#ifndef STUDENT
#include "scheduler.h"
#include "program.h"
#include "tests/tests.h"
#include "vmm/vmm.h"
#include "vmm/hmm.h"
#include "vmm/shm.h"
#include "syscall.h"
#include "timer.h"
#include "kbd.h"

// Programme par défaut en fonction de la machine, cf kernel/bot/$(MACHINE)/setup.c
extern char * default_program;



int64_t launcher(void *arg)
{
	int pid;
	printf("Launcher start.\n");
	if ((pid = sched_ustart(default_program, 1024, 11, (void *) 0)) < 0) {
		assert(0 && "Unable to run default machine app\n");
	}
	return sched_waitpid(pid, NULL);
}

int64_t kernel_tests(void *arg)
{
	puts("Tests kernel");
	int pid = sched_kstart(test0, 10, "Test 0", 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test1, 10, "test1", 0);
	sched_waitpid(pid, NULL);
	puts("Kernel tests done.");
	return 0;
}

#endif // STUDENT

int kernel_start()
{
	splash_screen();
	splash_vga_screen();

#ifndef STUDENT
	printf("Keyboard initialization...");
	kbd_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Hardware memory manager initialization...");
	extern char _free_memory_start[];
	extern char _memory_end[];
	hmm_init(_free_memory_start, _memory_end);
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");
	printf("\tNumber of frames %d = %d ko\n", hmm_frame_count(), hmm_free_memory() >> 10);

	printf("Clock interruptions...");
	clock_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Virtual memory manager initialization...");
	init_virtual_memory();
	if (is_virtual_memory_enable()) {
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		printf("Shared memory API initialization...");
		shm_init();
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		printf("Syscall initialization...");
		sysc_init();
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		sched_kstart(launcher, 10, "Launcher", 0);

	} else {
                printf("\r\t\t\t\t\t\t\t[  \x1b[9mNOT AVAILABLE\x1b[0m  ]\n");
		printf("# Virtual memory not available, start kernel tests\n");
		sched_kstart(kernel_tests, 10, "Launcher", 0);
	}

#endif // STUDENT
	puts("Hello world!");
exit(0);
}
