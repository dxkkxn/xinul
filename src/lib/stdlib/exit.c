// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <device.h>

void exit(int status)
{
    poweroff_dev->poweroff(status);
    __asm__ __volatile__("1: j 1b");
    __builtin_unreachable();
}
