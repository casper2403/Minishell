/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:27 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:22:59 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (*last_exit = builtin_pwd(token->argv, env));
	if (ft_strcmp(token->argv[0], "export") == 0)
		return (*last_exit = builtin_export(token->argv, env));
	if (ft_strcmp(token->argv[0], "unset") == 0)
		return (*last_exit = builtin_unset(token->argv, env));
	if (ft_strcmp(token->argv[0], "env") == 0)
		return (*last_exit = builtin_env(token->argv, env));
	if (ft_strcmp(token->argv[0], "exit") == 0)
		return (*last_exit = builtin_exit(token->argv, *env, last_exit));
	return (*last_exit = 0);
}

// For non-existent files with path
void	no_such_file_exit(char *cmd, char *path)
{
	(void)path;
	write(2, cmd, ft_strlen(cmd));
	write(2, ": No such file or directoryy\n", 29);
	exit(127);
}

// For directories with path
void	is_dir_exit(char *cmd, char *path)
{
	(void)path;
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Is a directory\n", 17);
	exit(126);
}

// For files without execute permissions with path
void	permission_denied_exit(char *cmd, char *path)
{
	(void)path;
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	exit(126);
}
