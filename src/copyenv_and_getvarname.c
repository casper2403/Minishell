/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyenv_and_getvarname.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:46:00 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 16:46:02 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

char	**copy_env(char **env)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

void	setup_child_io(struct s_piper *piper)
{
	if (piper->in_fd != STDIN_FILENO)
	{
		dup2(piper->in_fd, STDIN_FILENO);
		close(piper->in_fd);
	}
	if (piper->out_fd != STDOUT_FILENO)
	{
		dup2(piper->out_fd, STDOUT_FILENO);
		close(piper->out_fd);
	}
	if (piper->pipe_fd[0] != -1 && piper->pipe_fd[0] != piper->in_fd)
		close(piper->pipe_fd[0]);
	if (piper->pipe_fd[1] != -1 && piper->pipe_fd[1] != piper->out_fd)
		close(piper->pipe_fd[1]);
}
