/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(char **cmd, int index)
{
	if (cmd[index][0] == '<')
	{
		if (cmd[index + 1] && cmd[index + 1][0] == '<')
			return (REDIR_HEREDOC);
		return (REDIR_IN);
	}
	if (cmd[index][0] == '>')
	{
		if (cmd[index + 1] && cmd[index + 1][0] == '>')
			return (REDIR_APPEND);
		return (REDIR_OUT);
	}
	return (REDIR_OUT);
}

// TODO detect quote on target if needed
t_redir	*create_redir(t_redir_type type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = strdup(target);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->quoted = false;
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_token *token, t_redir *new_redir)
{
	t_redir	*curr;

	if (!new_redir)
		return ;
	if (!token->redirs)
		token->redirs = new_redir;
	else
	{
		curr = token->redirs;
		while (curr->next)
			curr = curr->next;
		curr->next = new_redir;
	}
}

int	remove_tokens_from_array(char ***cmd_arr, int start, int cnt)
{
	char	**arr;
	int		len;
	int		i;

	arr = *cmd_arr;
	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	if (start < 0 || start + cnt > len)
		return (0);
	i = -1;
	while (++i < cnt)
		free(arr[start + i]);
	i = start - 1;
	while (++i < len - cnt)
		arr[i] = arr[i + cnt];
	while (i < len)
		arr[i++] = NULL;
	return (1);
}
