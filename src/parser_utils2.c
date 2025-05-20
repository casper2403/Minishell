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

	if (!in_quotes[0] && !in_quotes[1] && (input[*i] == '>'
			|| input[*i] == '<'))
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

void	handle_not_found_error(char *cmd, char *path, int has_slash)
{
	if (has_slash)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20);
	}
	if (path)
		free(path);
	exit(127);
}

void	handle_other_error(char *cmd, char *path)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	perror("");
	if (path)
		free(path);
	exit(126);
}
