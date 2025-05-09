/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:29:32 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 20:29:34 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_env_var(char *var, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	update_existing_var(char *var, char ***env, int index)
{
	char	*new_var;

	new_var = ft_strdup(var);
	if (!new_var)
		return (1);
	free((*env)[index]);
	(*env)[index] = new_var;
	return (0);
}

int	create_new_env_array(char *var, char ***env, int env_size)
{
	char	**new_env;
	int		j;

	new_env = malloc((env_size + 2) * sizeof(char *));
	if (!new_env)
		return (1);
	j = 0;
	while (j < env_size)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[env_size] = ft_strdup(var);
	if (!new_env[env_size])
		return (free(new_env), 1);
	new_env[env_size + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

int	add_env_var(char *var, char ***env)
{
	int		index;
	int		i;

	index = find_env_var(var, *env);
	if (index >= 0)
		return (update_existing_var(var, env, index));
	i = 0;
	while ((*env)[i])
		i++;
	return (create_new_env_array(var, env, i));
}
