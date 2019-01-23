// See LICENSE for license details.

#include <stdio.h>
#include <device.h>

int getchar()
{
	int i;
	while ((i = console_dev->getchar()) <= 255);
    return i;
}
