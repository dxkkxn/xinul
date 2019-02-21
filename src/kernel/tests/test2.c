#include "tests.h"

#include "stdio.h"
#include "assert.h"

#include "scheduler.h"

int64_t proc_kill(void* arg)
{
	printf(" X");
	return (int64_t)arg;
}

int64_t proc_exit(void* arg)
{
	printf(" 5");
	sched_exit((int64_t) arg);
	assert(0);
	return 0;
}

int64_t test2(void* arg)
{
	int64_t rval;
	int64_t r;
	int64_t pid1;
	int64_t val = 45;

	(void) arg;
	
	printf("test2... ");
	printf("1");
	pid1 = sched_kstart(proc_kill, 100, "proc_kill", (void *) val);
	assert(pid1 > 0);
	printf(" 2");
	r = sched_kill(pid1);
	assert(r == 0);
	printf(" 3");
	r = sched_waitpid(pid1, &rval);
	assert(r == pid1);
	printf(" 4");
	pid1 = sched_kstart(proc_exit, 192, "proc_exit", (void *) val);
	assert(pid1 > 0);
	printf(" 6");
	r = sched_waitpid(pid1, &rval);
	assert(rval == val);
	assert(r == pid1);
	assert(sched_waitpid(sched_get_active_pid(), &rval) < 0);
	printf(" 7.");

	printf("\r\t\t\t\t\t\t\t[OK]\n");

	return 0;
}
