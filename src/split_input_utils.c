/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:00:21 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 18:00:24 by cstevens         ###   ########.fr       */
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

void	handle_quotes(char c, bool *quotes)
{
	if (c == '\'' && !quotes[1])
		quotes[0] = !quotes[0];
	else if (c == '\"' && !quotes[0])
		quotes[1] = !quotes[1];
}

int	handle_invalid(char **tokenized, int j)
{
	while (j > 0)
		free(tokenized[--j]);
	return (0);
}

int	handle_pipe(char *input, char **tokenized, int *indices)
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

int	pipe_syntax_error(void)
{
	write(2, " syntax error near unexpected token `|'\n", 39);
	return (2);
}
