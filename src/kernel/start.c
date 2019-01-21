#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "scheduler.h"
#include "program.h"
#include "interrupts.h"
#include "vmm.h"
#include "hmm.h"
#include "mapper.h"
#include "syscall.h"

int main()
{
	printf("\n= OSON Initialization =\n");

	printf("Clock interruptions...");
//	setup_clock_interrupts();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Hardware memory manager initialization...");
	extern char _free_memory_start[];
	extern char _memory_end[];
	hmm_init(_free_memory_start, _memory_end);
	printf("\r\t\t\t\t\t\t\t[OK]\n");
	printf("Number of frames %d = %d ko\n", hmm_frame_count(), hmm_free_memory() >> 10);

	printf("Virtual memory manager initialization...");
	init_virtual_memory();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Syscall initialization...");
	sysc_init();
	printf("\r\t\t\t\t\t\t\t[OK]\n");

	printf("Start test program hello\n");
	if (sched_ustart("hello", 1024, 10, (void *) 0) < 0) {
		assert(0 && "Unable to create programme hello userde test ");
	}
	//if (sched_kstart(systemd, 1, "systemd", (void *) 0) < 0) {
//		assert(0 && "Unable to create systemd process");
//	}
	return 0;
}
