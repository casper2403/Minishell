/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_counting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// split by spaces, keep track of quotes
// i think this is right
// look for edge cases
// goes into infinite loop when given < or >

static void	update_quotes(char c, bool *in_s, bool *in_d)
{
	if (c == '\'' && !(*in_d))
		*in_s = !(*in_s);
	else if (c == '\"' && !(*in_s))
		*in_d = !(*in_d);
}

static bool	is_splitter(char c, bool in_s, bool in_d)
{
	return (!in_s && !in_d && (c == ' ' || c == '<' || c == '>'));
}

int	count_args(char *cmd)
{
	int		i;
	int		count;
	bool	in_s;
	bool	in_d;
	bool	in_word;

	i = 0;
	count = 0;
	in_s = false;
	in_d = false;
	in_word = false;
	while (cmd[i])
	{
		if (is_splitter(cmd[i], in_s, in_d))
		{
			if (cmd[i] != ' ')
				count++;
			in_word = false;
		}
		else if (!in_word && ++count)
			in_word = true;
		update_quotes(cmd[i++], &in_s, &in_d);
	}
	return (count);
}

int	arraylen(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

// TODO
// incorrect! should keep track of quotes!
// check if full fixed
int	count_pipes(char *input)
{
	int		i;
	int		count;
	bool	in_s_quote;
	bool	in_d_quote;

	in_s_quote = 0;
	in_d_quote = 0;
	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '|' && !in_s_quote && !in_d_quote)
			count++;
		if (input[i] == '\'' && !in_d_quote)
			in_s_quote = !in_s_quote;
		if (input[i] == '\"' && !in_s_quote)
			in_d_quote = !in_d_quote;
		i++;
	}
	return (count);
}
