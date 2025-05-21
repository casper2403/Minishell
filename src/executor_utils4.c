/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:40 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:22:10 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_error(t_redir *r)
{
	char	*str;

	str = ft_strjoin("minishell: ", r->file);
	if (!str)
		return ;
	ft_putstr_fd(str, 2);
	free(str);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}

int	setup_file_redirection(t_redir *r, int *last_exit, char **env)
{
	int		fd;
	char	*expanded_file;

	if (!r->quoted)
	{
		expanded_file = expand_variables(r->file, *last_exit, env);
		if (!expanded_file)
			return (fd_error(r), *last_exit = 1, -1);
	}
	else
		expanded_file = ft_strdup(r->file);
	if (r->type == REDIR_IN)
		fd = open(expanded_file, O_RDONLY);
	else if (r->type == REDIR_OUT)
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(expanded_file);
	if (fd < 0)
	{
		fd_error(r);
		*last_exit = 1;
		return (-1);
	}
	return (fd);
}

int	setup_redirections(t_redir *r, int *last_exit, char **env)
{
	int	fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			fd = handle_heredoc(r, *last_exit, env);
			if (fd == -1)
				return (1);
		}
		else
		{
			fd = setup_file_redirection(r, last_exit, env);
			if (fd == -1)
				return (1);
		}
		if (r->type == REDIR_IN || r->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
	return (0);
}

void	handle_empty_args(struct s_token *token)
{
	int	i;

	while (token->argv[0] && token->argv[0][0] == '\0' && token->argv[1])
	{
		i = 0;
		while (token->argv[i + 1])
		{
			token->argv[i] = token->argv[i + 1];
			i++;
		}
		token->argv[i] = NULL;
	}
}

void	handle_command_error(char *cmd, char *path, int error_code)
{
	if (error_code == ENOENT)
		handle_not_found_error(cmd, path, ft_strchr(cmd, '/') != NULL);
	else if (error_code == EACCES)
		permission_denied_exit(cmd, path);
	else
		handle_other_error(cmd, path);
}
