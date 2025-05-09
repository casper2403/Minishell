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

static void	handle_input(char *input, int *last_exit, char ***local_env)
{
	if (!input)
	{
		write(1, "exit\n", 5);
		return ;
	}
	if (*input)
	{
		add_history(input);
		process_input(input, last_exit, local_env);
	}
	free(input);
}

static int	initialize_shell(char **env, char ***local_env)
{
	*local_env = copy_env(env);
	if (!*local_env)
	{
		write(2, "Failed to initialize environment\n", 31);
		return (1);
	}
	setup_signals();
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	char	**local_env;
	int		last_exit;

	last_exit = 0;
	if (initialize_shell(env, &local_env))
		return (1);
	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell> ");
		handle_input(input, &last_exit, &local_env);
		if (!input)
			break ;
	}
	free_env(local_env);
	rl_clear_history();
	return (0);
}
