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

int64_t launcher(void *arg)
{
	printf("Launcher start.\n");
	if (sched_ustart("autotest", 1024, 11, (void *) 0) < 0) {
		assert(0 && "Unable to run app autotest\n");
	}


//	printf("Nombre de frame disponible avant app hello : %d\n", hmm_frame_count());
//	if (sched_ustart("hello", 1024, 11, (void *) 0) < 0) {
//		assert(0 && "Unable to create programme hello userde test ");
//	}
//	printf("Nombre de frame disponible aprèes app hello : %d\n", hmm_frame_count());
	return 0;
}

int main()
{
	printf("\n= OSON Initialization =\n");

	printf("Clock interruptions...");
	setup_clock_interrupts();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

#if 0
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
#endif

	int i = -1;
	printf("mip: 0x%lx\n", csr_read(sip));
	i = getchar();
	printf("%c", i);
	printf("mip: 0x%x\n", 1234);
	//if (sched_kstart(systemd, 1, "systemd", (void *) 0) < 0) {
//		assert(0 && "Unable to create systemd process");
//	}
	assert(0 && "end of main");
}
