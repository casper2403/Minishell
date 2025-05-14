/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:56:42 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 16:56:45 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prs_handle_quotes(char c, bool *in_quotes)
{
	if (c == '\'' && !in_quotes[1])
		in_quotes[0] = !in_quotes[0];
	else if (c == '\"' && !in_quotes[0])
		in_quotes[1] = !in_quotes[1];
}

int	prs_process_redirection(char *input, int *i, bool *in_quotes)
{
	int	status;

	if (!in_quotes[0] && !in_quotes[1]
		&& (input[*i] == '>' || input[*i] == '<'))
	{
		status = check_redir_count(input, i);
		if (status != 0)
			return (status);
		status = check_after_redir(input, i);
		if (status != 0)
			return (status);
		return (1);
	}
	return (0);
}
