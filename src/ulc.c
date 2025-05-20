/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ulc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvandebe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:50:15 by pvandebe          #+#    #+#             */
/*   Updated: 2025/05/20 18:50:39 by pvandebe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	find_env_var_index_recursive(char **env, const char *prefix, int i)
{
	if (!env[i])
		return (-1);
	if (ft_strncmp(env[i], prefix, ft_strlen(prefix)) == 0)
		return (i);
	return (find_env_var_index_recursive(env, prefix, i + 1));
}

static void	copy_env_recursive(char **src, char **dest, int i, int size)
{
	if (i == size)
		return ;
	dest[i] = src[i];
	copy_env_recursive(src, dest, i + 1, size);
}

static int	get_env_size_recursive(char **env, int i)
{
	if (!env[i])
		return (i);
	return (get_env_size_recursive(env, i + 1));
}

void	update_last_command(char ***env, const char *last_command)
{
	int		index;
	int		env_size;
	char	*new_value;
	char	**new_env;

	new_value = ft_strjoin("_=", last_command);
	if (!new_value)
		return ;
	index = find_env_var_index_recursive(*env, "_=", 0);
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = new_value;
		return ;
	}
	env_size = get_env_size_recursive(*env, 0);
	new_env = malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (free(new_value));
	copy_env_recursive(*env, new_env, 0, env_size);
	new_env[env_size] = new_value;
	new_env[env_size + 1] = NULL;
	free(*env);
	*env = new_env;
}

void	update_cmd(struct s_token *tok, char ***env)
{
	const char	*last_arg;
	char		*path;

	last_arg = get_last_arg(tok);
	if (!last_arg)
		return ;
	if (ft_strchr(tok->argv[0], '/'))
		update_last_command(env, last_arg);
	else
	{
		path = find_executable(tok->argv[0], *env);
		if (path)
		{
			if (ft_strcmp(tok->argv[0], last_arg) == 0)
				update_last_command(env, path);
			else
				update_last_command(env, last_arg);
			free(path);
		}
		else
			update_last_command(env, last_arg);
	}
}
