#include "minishell.h"

static volatile int signal_received = 0;

void ctrl_c_handler(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void ctrl_backslash_handler(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void signal_handler(int signalnumber)
{
	if (signal_received)
		return;
	signal_received = 1;
	if (signalnumber == SIGINT)
		ctrl_c_handler();
	if (signalnumber == SIGQUIT)
		ctrl_backslash_handler();
	signal_received = 0;
}


int main() {
	char *input;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (process_input(input))
			return (printf("bad input.\n"));
		if (*input)
			add_history(input);
		free(input);
	}
	rl_clear_history();
	return 0;
}
