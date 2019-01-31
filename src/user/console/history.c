#include "shell.h"
#include "stdbool.h"
#include "stdio.h"

int read_index = 0;
int write_index = 0;

char history[HISTORY_SIZE][LINE_SIZE];

void clear_history() {
	read_index = write_index;
}

void save_command(char *command) {
	sprintf(history[write_index], "%s", command);
	write_index = (write_index + 1) % HISTORY_SIZE;
	if (read_index == write_index) {
		read_index = (read_index + 1) % HISTORY_SIZE;
	}
}

void display_history() {
	int i = read_index;
	while (i != write_index) {
		printf(" - %s\n", history[i]);
		i = (i + 1) % HISTORY_SIZE;
	}
}
