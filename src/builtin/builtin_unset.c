/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:37 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:50:38 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_var_name(char *name)
{
	int	i;

	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	find_env_var(char *var_name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static int	remove_env_var(char *var_name, char ***env)
{
	int		ix;
	int		i;
	int		j;
	char	**new_env;

	ix = find_env_var(var_name, *env);
	if (ix < 0)
		return (0);
	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc(i * sizeof(char *));
	if (!new_env)
		return (1);
	j = 0;
	i = 0;
	while ((*env)[i])
	{
		if (i != ix)
			new_env[j++] = (*env)[i];
		i++;
	}
	return (new_env[j] = NULL, free((*env)[ix]), free(*env), *env = new_env, 0);
}

int	builtin_unset(char **argv, char ***env)
{
	int	i;
	int	ret;

	ret = 0;
	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_var_name(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		else if (remove_env_var(argv[i], env))
			ret = 1;
		i++;
	}
	return (ret);
}
