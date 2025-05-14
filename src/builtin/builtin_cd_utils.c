/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:55:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 19:55:25 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_target_path(char **argv, char **env)
{
	char	*home;
	char	*oldpwd;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		home = env_get("HOME", env);
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (ft_strdup(home));
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		oldpwd = env_get("OLDPWD", env);
		if (!oldpwd)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putendl_fd(oldpwd, 1);
		return (ft_strdup(oldpwd));
	}
	return (expand_variables(argv[1], 0, env));
}

void	update_env_var(char ***env, char *var, char *value)
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
