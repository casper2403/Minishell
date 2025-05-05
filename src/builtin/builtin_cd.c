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

static char	*get_target_path(char **argv)
{
	char	*home;
	char	*oldpwd;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (ft_strdup(home));
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		oldpwd = getenv("OLDPWD");
		if (!oldpwd)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putendl_fd(oldpwd, 1);
		return (ft_strdup(oldpwd));
	}
	return (expand_variables(argv[1], 0));
}

static void	update_env_var(char ***env, char *var, char *value)
{
	char	*new_var;
	char	*tmp;
	int		i;

	i = 0;
	new_var = ft_strjoin(var, "=");
	tmp = new_var;
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], var, ft_strlen(var)) == 0
			&& (*env)[i][ft_strlen(var)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_var;
			return ;
		}
		i++;
	}
	(*env)[i] = new_var;
	(*env)[i + 1] = NULL;
}

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
	path = get_target_path(argv);
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
