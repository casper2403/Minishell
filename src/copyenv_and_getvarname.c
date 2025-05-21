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

void	initialize_default_env(char ***local_env)
{
	char	cwd[1024];
	char	*default_path;
	char	*system_path;

	default_path = "/usr/bin:/bin";
	if (!getcwd(cwd, sizeof(cwd)))
		return ;
	system_path = getenv("PATH");
	if (!system_path)
		system_path = default_path;
	*local_env = malloc(sizeof(char *) * 5);
	if (!*local_env)
		return ;
	(*local_env)[0] = ft_strjoin("PWD=", cwd);
	(*local_env)[1] = ft_strdup("SHLVL=1");
	(*local_env)[2] = ft_strjoin("PATH=", system_path);
	(*local_env)[3] = ft_strdup("_=./minishell");
	(*local_env)[4] = NULL;
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
}

char	*get_token_type(char *input, int *i)
{
	if (!input[*i])
		return ("newline");
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
			return (">>");
		return (">");
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			return ("<<");
		return ("<");
	}
	return ("|");
}
