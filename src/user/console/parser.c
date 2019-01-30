#include "shell.h"
#include "stdio.h"

void parse_cmd(char *line, char cmd[CMD_ARGS][LINE_SIZE], int *bool_bg) {
	int arg_index = 0;
	int line_index = 0;
	int word_index = 0;
	char char_prec = ' ';
	while (line[line_index] != '\0') {
		if (line[line_index] == ' ') {
			if (char_prec != ' ') {
				snprintf(cmd[arg_index], word_index, "%s", &(line[line_index - word_index]));
				arg_index++;
			}
			word_index = -1;
		}
		char_prec = line[line_index];
		word_index++;
		line_index++;
	}
	if (arg_index > 0 && line_index > 1
			&& line[line_index-1] == '&' && line[line_index-2] == ' ') {
		*bool_bg = 1;
	} else {
		snprintf(cmd[arg_index], word_index, "%s", &(line[line_index - word_index]));
	}
}
