/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:06 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:08 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile int	g_signal_received = 0;

void	ctrl_c_handler(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ctrl_backslash_handler(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_handler(int signalnumber)
{
	if (g_signal_received)
		return ;
	g_signal_received = 1;
	if (signalnumber == SIGINT)
		ctrl_c_handler();
	if (signalnumber == SIGQUIT)
		ctrl_backslash_handler();
	g_signal_received = 0;
}

// CHANGE sigaction gebruikt ipv signal.

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	memset(&sa_int, 0, sizeof(struct sigaction));
	memset(&sa_quit, 0, sizeof(struct sigaction));
	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction SIGINT");
		exit(1);
	}
	sa_quit.sa_handler = signal_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		exit(1);
	}
}

static char	**copy_env(char **env)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

static void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	char	**local_env;
	int		last_exit;

	last_exit = 0;
	local_env = copy_env(env);
	if (!local_env)
	{
		write(2, "Failed to initialize environment\n", 31);
		exit(1);
	}
	(void)argc;
	(void)argv;
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*input)
		{
			add_history(input);
			process_input(input, &last_exit, &local_env);
		}
		free(input);
	}
	free_env(local_env);
	rl_clear_history();
	return (0);
}
