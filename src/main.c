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

void	free_env(char **env)
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

static void	increment_shlvl(char ***local_env)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;
	char	*export_arg[3];
	char	*new_value;

	shlvl_str = env_get("SHLVL", *local_env);
	if (!shlvl_str || !*shlvl_str)
		shlvl = 1;
	else
		shlvl = ft_atoi(shlvl_str) + 1;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	new_value = ft_strjoin("SHLVL=", new_shlvl);
	free(new_shlvl);
	if (!new_value)
		return ;
	export_arg[0] = "export";
	export_arg[1] = new_value;
	export_arg[2] = NULL;
	builtin_export(export_arg, local_env);
	free(new_value);
}

static int	initialize_shell(char **env, char ***local_env)
{
	if (!env || !*env)
		initialize_default_env(local_env);
	else
		*local_env = copy_env(env);
	if (!*local_env)
	{
		write(2, "Failed to initialize environment\n", 31);
		return (1);
	}
	increment_shlvl(local_env);
	setup_signals(0);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	char	**local_env;
	int		last_exit;

	if (argv[1])
		return (printf("minishell: %s: No such file or directory\n", argv[1]),
			1);
	last_exit = 0;
	if (initialize_shell(env, &local_env))
		return (1);
	(void)argc;
	(void)argv;
	while (1)
	{
		if (last_exit == 6969 || last_exit < 0)
			break ;
		input = readline("minishell> ");
		handle_input(input, &last_exit, &local_env);
		if (!input)
			break ;
	}
	free_env(local_env);
	rl_clear_history();
	return (return_right_exit_value(last_exit));
}
