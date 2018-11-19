

#include "stdio.h"

#include "process.h"
#include "program.h"

int hello()
{
	printf("Hello words! My pid is %d\n", getpid());
	return 0;
}
