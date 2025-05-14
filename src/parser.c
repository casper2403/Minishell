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
	{
		free_char_array(divided_input);
		return (NULL);
	}
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

static int	check_redir_syntax(char *input)
{
	int		i;
	int		status;
	bool	in_quotes[2];

	i = 0;
	in_quotes[0] = false;
	in_quotes[1] = false;
	while (input[i] && input[i] == ' ')
		i++;
	while (input[i])
	{
		prs_handle_quotes(input[i], in_quotes);
		status = prs_process_redirection(input, &i, in_quotes);
		if (status > 0)
			continue ;
		else if (status < 0)
			return (status);
		i++;
	}
	return (0);
}

int	process_input(char *input, int *last_exit, char ***env)
{
	struct s_token	**tokens;
	int				syntax_check;

	if (!input || !*input)
		return (0);
	syntax_check = check_pipe_syntax(input);
	if (syntax_check != 0)
		return (*last_exit = syntax_check);
	syntax_check = check_redir_syntax(input);
	if (syntax_check != 0)
		return (*last_exit = syntax_check);
	tokens = tokenizer(input);
	if (!tokens)
		return (*last_exit = 1);
	tokens = parser(tokens);
	if (!tokens)
		return (*last_exit = 1);
	*last_exit = executor(tokens, last_exit, env);
	free_lexer(tokens, count_pipes(input) + 1);
	return (*last_exit);
}
