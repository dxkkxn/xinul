#ifndef SHELL_H
#define SHELL_H

#define HISTORY_SIZE 30
#define LINE_SIZE 50
#define CMD_ARGS 2
#define ARG_SIZE 10

// HISTORY
void clear_history();

void save_command(char *command);

void display_history();

// HELP
void usage(void);

// PARSER
void parse_cmd(char *line, char cmd[CMD_ARGS][LINE_SIZE], int *bool_bg);

// ECHO
void echo(char cmd[CMD_ARGS][LINE_SIZE]);

#endif
