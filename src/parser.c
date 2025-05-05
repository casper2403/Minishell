/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_token	*lexer(char *split_by_pipe)
{
	struct s_token	*token;

	token = malloc(sizeof(struct s_token));
	if (!token)
		return (NULL);
	token->redirs = NULL;
	token->next = NULL;
	token->built_in = false;
	token->argv = malloc((count_args(split_by_pipe) + 1) * sizeof(char *));
	if (!token->argv)
	{
		free(token);
		return (NULL);
	}
	if (split_arguments(split_by_pipe, token->argv, &token->quoted))
	{
		write(1, "unclosed quotes\n", 16);
		free(token->argv);
		free(token->quoted);
		free(token);
		return (NULL);
	}
	cut_redirs(token->argv, token);
	return (token);
}

// CHANGE extra loops toegevoegd voor de memory cleanup.
// TODO for loop norm
struct s_token	**tokenizer(char *input)
{
	int				i;
	char			**divided_input;
	struct s_token	**tokens;

	i = 0;
	divided_input = split_input(input);
	if (!divided_input)
		return (NULL);
	tokens = malloc((count_pipes(input) + 2) * sizeof(struct s_token *));
	if (!tokens)
		return (free_char_array(divided_input), NULL);
	while (divided_input[i])
	{
		tokens[i] = lexer(divided_input[i]);
		if (!tokens[i])
			return (free_char_array(divided_input), NULL);
		free(divided_input[i++]);
	}
	free(divided_input);
	tokens[i] = NULL;
	return (tokens);
}

static int	validate_redir(t_redir *redir, struct s_token **tokens, int i)
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

struct s_token	**parser(struct s_token **tokens)
{
	int			i;
	t_redir		*redir;

	if (!tokens)
		return (NULL);
	i = -1;
	while (tokens[++i])
	{
		if (tokens[i]->argv && tokens[i]->argv[0])
			tokens[i]->built_in = is_builtin(tokens[i]->argv[0]);
		redir = tokens[i]->redirs;
		if (!validate_redir(redir, tokens, i))
			return (NULL);
	}
	return (tokens);
}

// TODO: Add cleanup for tokens after execution
int	process_input(char *input, int *last_exit, char ***env)
{
	struct s_token	**tokens;

	tokens = tokenizer(input);
	if (!tokens)
	{
		*last_exit = 1;
		return (1);
	}
	tokens = parser(tokens);
	if (!tokens)
	{
		*last_exit = 1;
		return (1);
	}
	executor(tokens, last_exit, env);
	return (0);
}
