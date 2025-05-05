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

int	split_input_prepare(char *input, char ***tokenized)
{
	*tokenized = malloc((count_pipes(input) + 2) * sizeof(char *));
	if (!*tokenized)
		return (0);
	return (1);
}

int	split_input_process(char *input, char **tokenized,
		int *indices, bool *quotes)
{
	int	i;
	int	j;
	int	start;

	i = indices[0];
	j = indices[1];
	start = indices[2];
	if (input[i] == '|' && !quotes[0] && !quotes[1])
	{
		tokenized[j++] = ft_substr(input, start, i - start);
		if (!tokenized[j - 1])
		{
			while (j > 0)
				free(tokenized[--j]);
			return (0);
		}
		start = i + 1;
	}
	else if ((input[i] == ';' || input[i] == '\\') && !quotes[0] && !quotes[1])
	{
		while (j > 0)
			free(tokenized[--j]);
		return (0);
	}
	else
	{
		if (input[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		if (input[i] == '\"' && !quotes[0])
			quotes[1] = !quotes[1];
	}
	indices[0] = i + 1;
	indices[1] = j;
	indices[2] = start;
	return (1);
}

// split by pipes, IF the pipe is not in quotes!!
char	**split_input(char *input)
{
	char	**tokenized;
	bool	quotes[2];
	int		indices[3];
	int		i;
	int		j;

	if (!split_input_prepare(input, &tokenized))
		return (NULL);
	quotes[0] = false;
	quotes[1] = false;
	indices[0] = 0;
	indices[1] = 0;
	indices[2] = 0;
	i = 0;
	j = 0;
	while (input[i])
	{
		indices[0] = i;
		if (!split_input_process(input, tokenized, indices, quotes))
		{
			free(tokenized);
			return (NULL);
		}
		i = indices[0];
		j = indices[1];
	}
	tokenized[j++] = ft_substr(input, indices[2], i - indices[2]);
	if (!tokenized[j - 1])
	{
		while (j > 0)
			free(tokenized[--j]);
		free(tokenized);
		return (NULL);
	}
	tokenized[j] = NULL;
	return (tokenized);
}
