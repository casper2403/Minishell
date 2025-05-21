/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:20:42 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/22 22:20:47 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	handle_empty_command(struct s_token *tok, int *last_exit,
								struct s_piper *piper)
{
	if (!tok->argv || !tok->argv[0])
	{
		*last_exit = 0;
		piper->pids[piper->i] = 0;
		return (1);
	}
	return (0);
}

void	shift_empty_first_arg(struct s_token *tok)
{
	int	i;

	i = 0;
	while (tok->argv[i + 1])
	{
		tok->argv[i] = tok->argv[i + 1];
		if (tok->quoted)
			tok->quoted[i] = tok->quoted[i + 1];
		i++;
	}
	tok->argv[i] = NULL;
	if (tok->quoted)
		tok->quoted[i] = false;
}

void	write_heredoc_line(char *line, t_heredoc_ctx *ctx)
{
	char	*exp;

	if (!ctx->quoted)
	{
		exp = expand_variables(line, ctx->last_exit, ctx->env);
		ft_putendl_fd(exp, ctx->fd);
		free(exp);
	}
	else
		ft_putendl_fd(line, ctx->fd);
	free(line);
}

int	process_heredoc_lines(t_redir *r, int last_exit,
								char **env, int pipe_fd[2])
{
	char			*line;
	t_heredoc_ctx	ctx;

	ctx.fd = pipe_fd[1];
	ctx.last_exit = last_exit;
	ctx.env = env;
	ctx.quoted = r->quoted;
	while (!g_heredoc_interrupt)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, r->file) == 0)
			break ;
		write_heredoc_line(line, &ctx);
	}
	if (g_heredoc_interrupt)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (line)
		free(line);
	return (0);
}
