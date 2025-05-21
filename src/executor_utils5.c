/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:47:08 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:22:59 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_empty_first_arg(struct s_token *tok, int *last_exit,
									struct s_piper *piper)
{
	if (tok->argv[1])
		shift_empty_first_arg(tok);
	else
	{
		*last_exit = 0;
		piper->pids[piper->i] = 0;
		return (1);
	}
	return (0);
}

int	handle_builtin(struct s_token *tok, int *last_exit,
							struct s_piper *piper, char ***env)
{
	if (piper->in_fd != STDIN_FILENO
		|| piper->out_fd != STDOUT_FILENO || tok->redirs)
	{
		piper->pids[piper->i] = fork();
		if (piper->pids[piper->i] == 0)
		{
			setup_child_io(piper);
			if (tok->redirs
				&& setup_redirections(tok->redirs, last_exit, *env))
				exit(*last_exit);
			execute_builtin(tok, last_exit, env);
			exit(*last_exit);
		}
	}
	else
	{
		update_last_command(env, tok->argv[0]);
		execute_builtin(tok, last_exit, env);
		piper->pids[piper->i] = 0;
	}
	return (0);
}

char	*resolve_command_path(struct s_token *tok, int *last_exit,
								char ***env)
{
	char		*path;
	struct stat	st;

	if (ft_strchr(tok->argv[0], '/'))
	{
		if (stat(tok->argv[0], &st) != 0)
			return (no_such_file(tok->argv[0], last_exit), NULL);
		if (S_ISDIR(st.st_mode))
			return (is_directory(tok->argv[0], last_exit), NULL);
		if (access(tok->argv[0], X_OK) != 0)
			return (permission_denied(tok->argv[0], last_exit), NULL);
		path = ft_strdup(tok->argv[0]);
	}
	else
	{
		path = handle_path_search(tok->argv[0], *env);
		if (!path)
			return (command_not_found(tok->argv[0], last_exit), NULL);
	}
	return (path);
}

int	execute_external(struct s_token *tok, int *last_exit,
							struct s_piper *piper, char ***env)
{
	char	*path;
	int		result;

	path = resolve_command_path(tok, last_exit, env);
	if (!path)
		return (1);
	if (tok->argv && tok->argv[0])
		update_cmd(tok, env);
	tok->exec_path = path;
	result = fork_and_execute(tok, last_exit, piper, env);
	free(tok->exec_path);
	tok->exec_path = NULL;
	if (result < 0)
		return (1);
	return (0);
}

int	execute_cmd(struct s_token *tok, int *last_exit, struct s_piper *piper,
				char ***env)
{
	if (tok->argv)
		expand_args(tok->argv, tok->quoted, *last_exit, *env);
	if (handle_empty_command(tok, last_exit, piper))
		return (0);
	if (tok->argv[0][0] == '\0')
	{
		if (handle_empty_first_arg(tok, last_exit, piper))
			return (0);
	}
	if (tok->argv[0])
		tok->built_in = is_builtin(tok->argv[0]);
	if (tok->built_in)
		return (handle_builtin(tok, last_exit, piper, env));
	return (execute_external(tok, last_exit, piper, env));
}
