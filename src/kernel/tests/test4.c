#include "tests.h"

#include "scheduler.h"
#include "csr.h"

static const int loop_count0 = 5000;
static const int loop_count1 = 10000;


// iteration with a nop to catch the pending irq
void test_it()
{
	uint64_t tmp_status;

	tmp_status = csr_read(sstatus);
	csr_set(sstatus, MSTATUS_SIE);
	__asm__ __volatile__("nop");
	csr_write(sstatus, tmp_status);
}

int64_t busy1(void* arg)
{
	(void) arg;
	while (1) {
		int i, j;
		
		printf(" A");
		for (i = 0; i < loop_count1; i++) {
			test_it();
			for(j = 0; j < loop_count0; j++);
		}
	}
	
	return 0;
}

// assume the process to suspend has a priority == 64
int64_t busy2(void* arg)
{
	int i;

	(void) arg;

	for (i = 0; i < 3; i++) {
		int k, j;
		
		printf(" B");
		for (k = 0; k < loop_count1; k++) {
			test_it();
			for(j = 0; j < loop_count0; j++);
		}
	}
	i = sched_chprio((int64_t) arg, 16);
	assert(i == 64);
	
	return 0;
}

int64_t test4(void *arg)
{
	uint64_t pid1, pid2;
	int r;
	int64_t busy1_arg = 0;

	(void)arg;

	printf("test4... ");

	assert(sched_get_active_prio() == 128);
	pid1 = sched_kstart(busy1, 64, "busy1", (void *) busy1_arg);
	assert(pid1 > 0);
	pid2 = sched_kstart(busy2, 64, "busy2", (void *) pid1);
	assert(pid2 > 0);
	printf("1 -");
	r = sched_chprio(sched_get_active_pid(), 32);
	assert(r == 128);
	printf(" - 2");
	r = sched_kill(pid1);
	assert(r == 0);
	assert(sched_waitpid(pid1, 0) == pid1);
	r = sched_kill(pid2);
	assert(r < 0); /* kill d'un processus zombie */
	assert(sched_waitpid(pid2, 0) == pid2);
	printf(" 3");
	r = sched_chprio(sched_get_active_pid(), 128);
	assert(r == 32);
	printf(" 4.");

	printf("\r\t\t\t\t\t\t\t[OK]\n");

	return 0;
}
