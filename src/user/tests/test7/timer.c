#include "sysapi.h"

int main(void *arg)
{
        volatile unsigned long *timer = NULL;
		printf("shm acquire\n");
        timer = shm_acquire("test7_shm");
		printf("shm acquire done\n");
        assert(timer != NULL);

        (void)arg;
        while (1) {
                unsigned long t = *timer + 1;
                *timer = t;
                while (*timer == t) test_it();
        }
        while (1);
        return 0;
}

