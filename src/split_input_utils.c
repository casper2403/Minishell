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
