/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:45:29 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 16:45:31 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_quotes(char c, bool *in_quotes)
{
	if (c == '\'' && !in_quotes[1])
		in_quotes[0] = !in_quotes[0];
	else if (c == '\"' && !in_quotes[0])
		in_quotes[1] = !in_quotes[1];
}

int	check_pipe_sequence(char *input, int *i, bool *empty_command)
{
	if (*empty_command)
		return (pipe_syntax_error());
	*empty_command = true;
	(*i)++;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	if (input[*i] == '|' || input[*i] == '\0')
		return (pipe_syntax_error());
	return (0);
}

void	init_pipe_check(int *i, bool *empty_command, bool in_quotes[2])
{
	*i = 0;
	*empty_command = true;
	in_quotes[0] = false;
	in_quotes[1] = false;
}

int	check_starting_pipe(char *input, int *i)
{
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	if (input[*i] == '|')
		return (pipe_syntax_error());
	return (0);
}
