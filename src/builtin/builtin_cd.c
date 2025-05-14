/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:45 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:48 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_args(char **argv)
{
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

static int	get_current_dir(char *buffer, size_t size)
{
	if (getcwd(buffer, size) == NULL)
	{
		ft_putstr_fd("minishell: cd: getcwd failed\n", 2);
		return (1);
	}
	return (0);
}

static int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

static void	update_environment(char ***env, char *oldpwd, char *newpwd)
{
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", newpwd);
}

int	builtin_cd(char **argv, char ***env)
{
	char	*path;
	char	oldpwd[4096];
	char	newpwd[4096];

	if (validate_args(argv))
		return (1);
	path = get_target_path(argv, *env);
	if (!path || path[0] == '\0')
		return (free(path), 1);
	if (get_current_dir(oldpwd, sizeof(oldpwd)))
		return (free(path), 1);
	if (change_directory(path))
		return (free(path), 1);
	if (get_current_dir(newpwd, sizeof(newpwd)))
		return (free(path), 1);
	update_environment(env, oldpwd, newpwd);
	free(path);
	return (0);
}
