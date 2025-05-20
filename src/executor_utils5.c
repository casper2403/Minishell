/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:47:08 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 15:47:11 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(struct s_token *token, char *path, char **env)
{
	struct stat	st;

	if (!path)
		command_not_found_exit(token->argv[0], NULL);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		is_dir_exit(token->argv[0], path);
	execve(path, token->argv, env);
	handle_command_error(token->argv[0], path, errno);
}

void	exec_child(struct s_token *token, int *last_exit, char ***env)
{
	char	*path;

	path = NULL;
	handle_empty_args(token);
	if (token->built_in)
		exit(execute_builtin(token, last_exit, env));
	if (!token->argv[0] || !token->argv[0][0])
		exit(0);
	if (ft_strchr(token->argv[0], '/'))
		fuck_the_norm(token, env);
	else
	{
		path = find_executable(token->argv[0], *env);
		if (!path)
			command_not_found_exit(token->argv[0], last_exit);
		execve(path, token->argv, *env);
		free(path);
		permission_denied_exit(token->argv[0], NULL);
	}
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
		setup_child_io(piper);
		if (token->redirs && setup_redirections(token->redirs, last_exit, *env))
			exit(*last_exit);
		exec_child(token, last_exit, env);
	}
	else if (piper->out_fd != STDOUT_FILENO)
		close(piper->out_fd);
	return (0);
}

int	prepare_fds(struct s_piper *piper, struct s_token **tokens)
{
	piper->pipe_fd[0] = -1;
	if (tokens[piper->i + 1])
	{
		if (pipe(piper->pipe_fd) != 0)
			return (1);
		piper->out_fd = piper->pipe_fd[1];
	}
	else
		piper->out_fd = STDOUT_FILENO;
	return (0);
}

int	execute_cmd(struct s_token *tok, int *last_exit, struct s_piper *piper,
		char ***env)
{
	if (tok->argv)
		expand_args(tok->argv, tok->quoted, *last_exit, *env);
	if (tok->built_in && piper->in_fd == STDIN_FILENO
		&& piper->out_fd == STDOUT_FILENO && !tok->redirs)
	{
		update_last_command(env, tok->argv[0]);
		execute_builtin(tok, last_exit, env);
		piper->pids[piper->i] = 0;
	}
	else
	{
		if (tok->argv && tok->argv[0])
			update_cmd(tok, env);
		if (fork_and_execute(tok, last_exit, piper, env) < 0)
			return (1);
	}
	return (0);
}
