/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:27 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 15:46:29 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_args(char **argv, bool *quote_type,
						int last_exit, char **env)
{
	int		i;
	char	*old;

	i = 0;
	while (argv[i])
	{
		if (quote_type && quote_type[i])
		{
			i++;
			continue ;
		}
		old = argv[i];
		argv[i] = expand_variables(old, last_exit, env);
		free(old);
		i++;
	}
}

int	execute_builtin(struct s_token *token, int *last_exit, char ***env)
{
	if (!token || !token->argv || !token->argv[0])
		return (*last_exit = 1);
	if (ft_strcmp(token->argv[0], "echo") == 0)
		return (*last_exit = builtin_echo(token->argv, token->quoted,
				*last_exit, *env));
	if (ft_strcmp(token->argv[0], "cd") == 0)
		return (*last_exit = builtin_cd(token->argv, env));
	if (ft_strcmp(token->argv[0], "pwd") == 0)
		return (*last_exit = builtin_pwd(token->argv));
	if (ft_strcmp(token->argv[0], "export") == 0)
		return (*last_exit = builtin_export(token->argv, env));
	if (ft_strcmp(token->argv[0], "unset") == 0)
		return (*last_exit = builtin_unset(token->argv, env));
	if (ft_strcmp(token->argv[0], "env") == 0)
		return (*last_exit = builtin_env(token->argv, env));
	if (ft_strcmp(token->argv[0], "exit") == 0)
		return (builtin_exit(token->argv));
	return (*last_exit = 0);
}

char	*search_in_dirs(char *cmd, char **dirs)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

void	command_not_found_exit(char *argv, int *last_exit)
{
	char	*la;

	if (ft_strcmp(argv, "$?") == 0)
	{
		la = ft_itoa(*last_exit);
		write(2, la, ft_strlen(la));
		free(la);
	}
	else
		write(2, argv, ft_strlen(argv));
	write(2, ": command not found\n", 20);
	exit(127);
}

void	is_dir_exit(char *argv, char *path)
{
	write(2, argv, ft_strlen(argv));
	write(2, ": Is a directory\n", 17);
	if (path)
		free(path);
	exit(126);
}
