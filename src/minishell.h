#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

typedef struct s_token
{
	char *text;
	bool command;
	bool flag;
	bool argument;
	bool redirection;
	bool pipe;
	bool env_variable;
	struct s_token *previous;
	struct s_token *next;
} t_token;

int process_input(char *input);

#endif
