/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:20:39 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:22:59 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// For non-existent files with path
int	no_such_file(char *cmd, int *last_exit)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": No such file or directory\n", 28);
	if (last_exit)
		*last_exit = 127;
	return (127);
}

// For directories with path
int	is_directory(char *cmd, int *last_exit)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Is a directory\n", 17);
	if (last_exit)
		*last_exit = 126;
	return (1);
}

// For files without execute permissions
int	permission_denied(char *cmd, int *last_exit)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	if (last_exit)
		*last_exit = 126;
	return (1);
}

int	fork_and_execute(struct s_token *token, int *last_exit,
					struct s_piper *piper, char ***env)
{
	setup_signals(1);
	piper->pids[piper->i] = fork();
	if (piper->pids[piper->i] == -1)
		return (-1);
	if (piper->pids[piper->i] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		setup_child_io(piper);
		if (token->redirs && setup_redirections(token->redirs, last_exit, *env))
			exit(*last_exit);
		execve(token->exec_path, token->argv, *env);
		perror("execve failed");
		exit(126);
	}
	else if (piper->out_fd != STDOUT_FILENO)
		close(piper->out_fd);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
