#include "shell.h"
#include "stdio.h"

void usage(void) {
	printf("GNU bat'sh, version 1.2.42(1)-release (x86-qemu-gnu)\n");
	printf("Ces commandes de shell sont definies de maniere interne. \nSaisissez \"help\" pour voir cette liste.\n");
	printf("	proc [&]\n");
	printf("	boot\n");
	printf("	clear\n");
	printf("	echo [on/off]\n");
	printf("	exit\n");
	printf("	help\n");
	printf("	history\n");
	printf("	kill [pid]\n");
	printf("	ps\n");
}
