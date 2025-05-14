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

void	permission_denied_exit(char *argv, char *path)
{
	write(2, argv, ft_strlen(argv));
	write(2, ": Permission denied\n", 20);
	if (path)
		free(path);
	exit(126);
}

char	*handle_pwd_var(char **env)
{
	char	*path;

	path = env_get("PWD", env);
	if (path && *path)
		return (ft_strdup(path));
	return (NULL);
}

char	*handle_absolute_path(char *cmd)
{
	struct stat	st;

	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		return (ft_strdup(cmd));
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (ft_strdup(cmd));
}

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
	if (!res && cmd[0] != '\0')
		return (ft_strdup(cmd));
	return (res);
}

char	*find_executable(char *cmd, char **env)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strcmp(cmd, "$PWD") == 0)
		return (handle_pwd_var(env));
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		return (handle_absolute_path(cmd));
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (handle_path_search(cmd, env));
}
