#include "tests.h"

#include "stdio.h"
#include "assert.h"

#include "scheduler.h"

int64_t dummy1(void* arg)
{
	printf("1");
	assert((int64_t) arg == DUMMY_VAL);

	return 3;
}

int64_t dummy2(void* arg)
{
	printf(" 5");
	assert((int64_t) arg == DUMMY_VAL + 1);

	return 4;
}

int64_t test1(void* arg)
{
	int64_t pid1;
	int64_t r;
	int64_t rval;

	(void) arg;

	printf("test1... ");
	pid1 = sched_kstart(dummy1, 192, "Dummy1", (void *) DUMMY_VAL);
	assert(pid1 > 0);
	printf(" 2");
	r = sched_waitpid(pid1, &rval);
	assert(r == pid1);
	assert(rval == 3);
	printf(" 3");
	pid1 = sched_kstart(dummy2, 100, "Dummy1", (void *) (DUMMY_VAL + 1));
	assert(pid1 > 0);
	printf(" 4");
	r = sched_waitpid(pid1, &rval);
	assert(r == pid1);
	assert(rval == 4);
	printf(" 6.");

	printf("\r\t\t\t\t\t\t\t[OK]\n");
	return 0;
}
