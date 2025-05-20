/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:40 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 15:46:41 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_redir *r, int last_exit, char **env)
{
	int		pipe_fd[2];
	char	*line;
	char	*exp;
	int		fd;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	setup_signals(2);
	while (!g_heredoc_interrupt)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, r->file) == 0)
			break ;
		if (!r->quoted)
		{
			exp = expand_variables(line, last_exit, env);
			ft_putendl_fd(exp, pipe_fd[1]);
			free(exp);
		}
		else
			ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	if (g_heredoc_interrupt)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;  // Return error on interrupt
    }
	free(line);
	close(pipe_fd[1]);
	setup_signals(0);
	return (close(pipe_fd[1]), fd = dup(pipe_fd[0]), close(pipe_fd[0]), fd);
}

void	fd_error(t_redir *r)
{
	ft_putstr_fd(ft_strjoin("minishell: ", r->file), 2);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}

int	setup_redirections(t_redir *r, int *last_exit, char **env)
{
	int	fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
			fd = handle_heredoc(r, *last_exit, env);
		if (fd == -1)
			return (1);
		else if (r->type == REDIR_IN)
			fd = open(r->file, O_RDONLY);
		else if (r->type == REDIR_OUT)
			fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (fd_error(r), *last_exit = 1, 1);
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
