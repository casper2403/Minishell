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
		return (ft_putendl_fd(oldpwd, 1), ft_strdup(oldpwd));
	}
	else
		return (ft_strdup(argv[1]));
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(result, s1, ft_strlen(s1) + 1);
	ft_strlcat(result, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	free(s1);
	return (result);
}

void	update_env_var(char ***env, char *var, char *value)
{
	char	*new_var;
	int		i;

	if (!var || !value)
		return ;
	new_var = ft_strjoin(var, "=");
	if (!new_var)
		return ;
	new_var = ft_strjoin_free(new_var, value);
	if (!new_var)
		return ;
	i = find_env_var(var, *env);
	if (i >= 0)
	{
		free((*env)[i]);
		(*env)[i] = new_var;
	}
	else
	{
		add_env_var(new_var, env);
		free(new_var);
	}
}
