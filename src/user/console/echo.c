#include "shell.h"
#include "syscall.h"
#include "stdio.h"
#include "string.h"

void echo(char cmd[CMD_ARGS][LINE_SIZE]) {
	if (strcmp(cmd[1], "on") == 0) {
		cons_echo(1);
		printf("mode echo on\n");
	} else if (strcmp(cmd[1], "off") == 0) {
		cons_echo(0);
		printf("mode echo off\n");
	} else {
		printf("usage: echo [on/off]\n");
	}
}
