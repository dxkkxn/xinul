// See LICENSE for license details.

#include <stdio.h>
#include <device.h>

__inline__ static int kputchar(int ch)
{
	return console_dev->putchar(ch);
}

int cons_write(const char *str, long size)
{
	int res = 0;
	for (int i = 0; i < size; i++) {
		if ((kputchar(str[i])) >= 0) {
			res++;
		} else
			break;
	}
	return res;
}

