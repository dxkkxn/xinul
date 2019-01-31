#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "scheduler.h"
#include "program.h"
#include "supervisor_trap.h"
#include "vmm.h"
#include "hmm.h"
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

int main()
{
	printf("\n= OSON Initialization =\n");

	printf("Clock interruptions...");
	clock_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

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

	printf("Virtual memory manager initialization...");
	init_virtual_memory();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Syscall initialization...");
	sysc_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	sched_kstart(launcher, 10, "Launcher", 0);
#if 0
	printf("sstatus: 0x%lx\n", csr_read(sstatus));
	csr_set(sie, 0x200);
	//printf("sie: 0x%lx\n", csr_read(sie));
	printf("plic: 0x%lx\n", *((uintptr_t*)0xc000000));
		printf("sie: 0x%lx\n", csr_read(sie));
		printf("sip: 0x%lx\n", csr_read(sip));
	int i = -1;
	while(1) {if (i%100000 == 0) printf("."); i++;}

#endif
	assert(0 && "end of main");
}
