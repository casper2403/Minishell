/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:48:13 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:48:18 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	update_fds(struct s_piper *piper, struct s_token **tokens)
{
	if (piper->in_fd != STDIN_FILENO)
		close(piper->in_fd);
	if (tokens[piper->i + 1])
		piper->in_fd = piper->pipe_fd[0];
	else
		piper->in_fd = STDIN_FILENO;
	if (piper->out_fd != STDOUT_FILENO)
		close(piper->out_fd);
}

int	execute_loop(struct s_piper *piper, struct s_token **tokens, int *last_exit,
	char ***env)
{
	piper->i = -1;
	while (++piper->i < piper->cmd_count)
	{
		if (prepare_fds(piper, tokens))
		{
			*last_exit = 1;
			return (1);
		}
		if (execute_cmd(tokens[piper->i], last_exit, piper, env))
		{
			*last_exit = 1;
			return (1);
		}
		update_fds(piper, tokens);
	}
	return (0);
}

static void	initialize_piper(t_piper *piper, struct s_token **tokens)
{
	piper->out_fd = STDOUT_FILENO;
	piper->in_fd = STDIN_FILENO;
	piper->cmd_count = 0;
	while (tokens[piper->cmd_count])
		piper->cmd_count++;
	piper->pids = malloc(piper->cmd_count * sizeof(pid_t));
}

static void	wait_for_children(t_piper *piper, int *last_exit)
{
	int	status;

	piper->i = -1;
	while (++piper->i < piper->cmd_count)
	{
		if (piper->pids[piper->i] > 0)
		{
			waitpid(piper->pids[piper->i], &status, 0);
			*last_exit = WEXITSTATUS(status);
		}
	}
	g_is_child_running = 0;
}

int	executor(struct s_token **tokens, int *last_exit, char ***env)
{
	t_piper	piper;

	initialize_piper(&piper, tokens);
	if (!piper.pids)
		return (*last_exit = 1, 1);
	if (execute_loop(&piper, tokens, last_exit, env))
		return (free(piper.pids), *last_exit);
	if (piper.in_fd != STDIN_FILENO)
		close(piper.in_fd);
	wait_for_children(&piper, last_exit);
	return (free(piper.pids), *last_exit);
}
