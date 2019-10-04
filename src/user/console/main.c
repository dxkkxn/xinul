
// Created by lea on 25/02/18.
//

#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "syscall.h"
#include "shell.h"

char command[LINE_SIZE + 1];


void process_cmd(char *command) {
	if (strcmp(command, "") != 0) {
		char cmd[CMD_ARGS][LINE_SIZE];
		int bool_bg=0;
		parse_cmd(command, cmd, &bool_bg);
		if (strcmp(cmd[0], "echo") == 0) {
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
		else if (strcmp(cmd[0], "exit") == 0) {
			exit(0);
		}
		else if (strcmp(cmd[0], "clear") == 0) {
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			//clear_screen();
		}
		else if (strcmp(cmd[0], "help") == 0) {
			usage();
		}
		else if (strcmp(cmd[0], "history") == 0){
			display_history();
		}
		else if (strcmp(cmd[0], "kill") == 0){
			int pid_to_kill=strtol(cmd[1], NULL, 10);
			if (pid_to_kill > 0){
				int i = kill(pid_to_kill);
				if (i == 0) {
					printf("[Killed] %i\n", pid_to_kill);
				}
			} else {
				printf("usage: kill [pid]\n");
			}
		}
		else {
			int pid = start(cmd[0], 4000, 128, (void *) cmd[1]);
			if (pid == -1) {
				printf("shell: %s: command not found\n", cmd[0]);
			} else if (!bool_bg) {
				waitpid(pid, NULL);
			} else {
				printf("[background] %i\n", pid);
			}
		}
	}
}


int main(void) {
	cons_echo(1);
	while (1) {
		printf("\x1b[95muser\x1b[91m@\x1b[34moson \x1b[0m$ ");
		cons_read(command, LINE_SIZE);
		process_cmd(command);
		save_command(command);
	}
}
