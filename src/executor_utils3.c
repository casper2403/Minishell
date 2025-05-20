/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:34 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 15:46:36 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_path_search(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*res;
	int		i;

	path = env_get("PATH", env);
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	res = search_in_dirs(cmd, dirs);
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
	return (res);
}

char	*find_executable(char *cmd, char **env)
{
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (handle_path_search(cmd, env));
}

void	expand_args(char **argv, bool *quote_type, int last_exit, char **env)
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

char	*handle_pwd_var(char **env)
{
	char	*path;

	path = env_get("PWD", env);
	if (path && *path)
		return (ft_strdup(path));
	return (NULL);
}
