#include "tests.h"

#include "scheduler.h"

int64_t waiter(void* arg)
{
	uint64_t pid = (uint64_t) arg;
	assert(sched_kill(pid) == 0);
	assert(sched_waitpid(pid, 0) < 0);

	return 1;
}

int64_t no_run(void* arg)
{
	(void) arg;
	assert(0);
	
	return 1;
}

int64_t test5(void *arg)
{
	int64_t pid1, pid2;
	int64_t r;

	(void)arg;

	printf("test5...");

	// Process 0 and priority 0 are invalid parameters
	assert(sched_kill(0) < 0);
	assert(sched_chprio(sched_get_active_pid(), 0) < 0);

	assert(sched_get_active_prio() == 128);
	pid1 = sched_kstart(no_run, 64, "no_run", (void *) 0);
	assert(pid1 > 0);

	assert(sched_kill(pid1) == 0);
	assert(sched_kill(pid1) < 0); // no zombie kill
	assert(sched_chprio(pid1, 128) < 0); // no chprio with a zombie
	assert(sched_chprio(pid1, 64) < 0); // no chprio with a zombie

	assert(sched_waitpid(pid1, 0) == pid1);
	assert(sched_waitpid(pid1, 0) < 0);

	pid1 = sched_kstart(no_run, 64, "no_run", (void *) 0);
	assert(pid1 > 0);
	pid2 = sched_kstart(waiter, 65, "waiter", (void *) pid1);
	assert(pid2 > 0);
	assert(sched_waitpid(pid2, &r) == pid2);
	assert(r == 1);
	assert(sched_waitpid(pid1, &r) == pid1);
	assert(r == 0);
	printf("ok.");

	printf("\r\t\t\t\t\t\t\t[OK]\n");

	return 0;
}
