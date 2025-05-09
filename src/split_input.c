/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_invalid(char **tokenized, int j)
{
	while (j > 0)
		free(tokenized[--j]);
	return (0);
}

static int	handle_pipe(char *input, char **tokenized, int *indices)
{
	int		i;
	int		j;
	int		start;
	char	*sub;

	i = indices[0];
	j = indices[1];
	start = indices[2];
	sub = ft_substr(input, start, i - start);
	if (!sub)
		return (handle_invalid(tokenized, j));
	tokenized[j++] = sub;
	indices[1] = j;
	indices[2] = i + 1;
	indices[0] = i + 1;
	return (1);
}

int	split_input_process(char *input, char **tokenized,
		int *indices, bool *quotes)
{
	int	i;
	int	j;

	i = indices[0];
	j = indices[1];
	if (input[i] == '|' && !quotes[0] && !quotes[1])
		return (handle_pipe(input, tokenized, indices));
	else if ((input[i] == ';' || input[i] == '\\') && !quotes[0] && !quotes[1])
		return (handle_invalid(tokenized, j));
	else
	{
		handle_quotes(input[i], quotes);
		indices[0] = i + 1;
		indices[1] = j;
	}
	return (1);
}

static int	add_last_token(char *input, char **tokenized, int *indices)
{
	int		j;
	int		start;
	char	*sub;

	j = indices[1];
	start = indices[2];
	sub = ft_substr(input, start, indices[0] - start);
	if (!sub)
	{
		while (j > 0)
			free(tokenized[--j]);
		return (0);
	}
	tokenized[j++] = sub;
	tokenized[j] = NULL;
	return (1);
}

char	**split_input(char *input)
{
	char	**tokenized;
	bool	quotes[2];
	int		indices[3];

	if (!split_input_prepare(input, &tokenized))
		return (NULL);
	quotes[0] = false;
	quotes[1] = false;
	indices[0] = 0;
	indices[1] = 0;
	indices[2] = 0;
	while (input[indices[0]])
	{
		if (!split_input_process(input, tokenized, indices, quotes))
		{
			free(tokenized);
			return (NULL);
		}
	}
	if (!add_last_token(input, tokenized, indices))
	{
		free(tokenized);
		return (NULL);
	}
	return (tokenized);
}
