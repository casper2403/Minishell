/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:30:21 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 16:30:22 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL};
	int			i;

	i = -1;
	while (builtins[++i])
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
	return (0);
}

int	validate_redir(t_redir *redir, struct s_token **tokens, int i)
{
	while (redir)
	{
		if (!redir->file || redir->file[0] == '\0')
			return (free_lexer(tokens, i), 0);
		if (redir->type == REDIR_HEREDOC)
		{
			if (ft_strchr(redir->file, ' ') || ft_strchr(redir->file, '\t'))
				return (free_lexer(tokens, i), 0);
			redir->quoted = (redir->file[0] == '\'' || redir->file[0] == '"');
		}
		redir = redir->next;
	}
	return (1);
}

void	print_redir_error(char *token)
{
	write(2, "minishell:", 10);
	if (!ft_strcmp(token, ">"))
		write(2, " syntax error near unexpected token `>'\n", 39);
	else if (!ft_strcmp(token, "<"))
		write(2, " syntax error near unexpected token `<'\n", 39);
	else
	{
		write(2, " syntax error near unexpected token `", 37);
		write(2, token, ft_strlen(token));
		write(1, "'", 1);
	}
	write(2, "\n", 1);
}

int	check_redir_count(char *input, int *i)
{
	int	redir_start;

	redir_start = *i;
	if (input[*i] == '>')
	{
		while (input[*i] == '>')
			(*i)++;
		if (*i - redir_start > 2)
		{
			print_redir_error(">");
			return (-2);
		}
	}
	else if (input[*i] == '<')
	{
		while (input[*i] == '<')
			(*i)++;
		if (*i - redir_start > 2)
		{
			print_redir_error("<");
			return (-2);
		}
	}
	return (0);
}

int	check_after_redir(char *input, int *i)
{
	char	*token;

	while (input[*i] && input[*i] == ' ')
		(*i)++;
	if (!input[*i] || input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
	{
		if (!input[*i])
			token = "newline";
		else if (input[*i] == '>')
			token = ">";
		else if (input[*i] == '<')
			token = "<";
		else
			token = "|";
		print_redir_error(token);
		return (-2);
	}
	return (0);
}
