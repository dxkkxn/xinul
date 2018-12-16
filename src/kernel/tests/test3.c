#include "tests.h"

#include "scheduler.h"

int64_t prio4(void* arg)
{
	// arg = priority of this proc
	int r;

	assert(sched_get_active_prio() == (int64_t) arg);
	printf("1");
	r = sched_chprio(sched_get_active_pid(), 64);
	assert(r == (int64_t) arg);
	printf(" 3");

	return 0;
}

int64_t prio5(void* arg)
{
	// arg = priority of this proc
	int r;

	assert(sched_get_active_prio() == (int64_t) arg);
	printf(" 7");
	r = sched_chprio(sched_get_active_pid(), 64);
	assert(r == (int64_t) arg);
	printf("error: I should have been killed\n");
	assert(0);

	return 0;
}

int64_t test3(void *arg)
{
	int pid1;
	int64_t p = 192;
	int r;

	(void)arg;

	printf("test3... ");

	assert(sched_get_active_prio() == 128);
	pid1 = sched_kstart(prio4, p, "prio4", (void *) p);
	assert(pid1 > 0);
	printf(" 2");
	r = sched_chprio(sched_get_active_pid(), 32);
	assert(r == 128);
	printf(" 4");
	r = sched_chprio(sched_get_active_pid(), 128);
	assert(r == 32);
	printf(" 5");
	assert(sched_waitpid(pid1, 0) == pid1);
	printf(" 6");

	assert(sched_get_active_prio() == 128);
	pid1 = sched_kstart(prio5, p, "prio4", (void *) p);
	assert(pid1 > 0);
	printf(" 8");
	r = sched_kill(pid1);
	assert(r == 0);
	assert(sched_waitpid(pid1, 0) == pid1);
	printf(" 9");
	r = sched_chprio(sched_get_active_pid(), 32);
	assert(r == 128);
	printf(" 10");
	r = sched_chprio(sched_get_active_pid(), 128);
	assert(r == 32);
	printf(" 11.");

	printf("\r\t\t\t\t\t\t\t[OK]\n");

	return 0;
}
