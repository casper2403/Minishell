/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:20:45 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:22:59 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_not_found(char *cmd, int *last_exit)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	if (last_exit)
		*last_exit = 127;
	return (127);
}

int	handle_heredoc(t_redir *r, int last_exit, char **env)
{
	int	pipe_fd[2];
	int	fd;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), -1);
	setup_signals(2);
	if (process_heredoc_lines(r, last_exit, env, pipe_fd) == -1)
		return (-1);
	close(pipe_fd[1]);
	setup_signals(0);
	fd = dup(pipe_fd[0]);
	close(pipe_fd[0]);
	return (fd);
}
