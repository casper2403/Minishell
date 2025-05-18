/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirection_helper(t_token *token,
		char *file, int count, char **cmd)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (free(file), 0);
	new->file = file;
	new->quoted = token->quoted[count - 1];
	new->next = NULL;
	if (count == 3)
	{
		if (cmd[0][0] == '<')
			new->type = REDIR_HEREDOC;
		else
			new->type = REDIR_APPEND;
	}
	else
	{
		if (cmd[0][0] == '<')
			new->type = REDIR_IN;
		else
			new->type = REDIR_OUT;
	}
	add_redir(token, new);
	return (1);
}

int	process_redirection(char ***command_array,
						struct s_token *token, int index, int count)
{
	char	*file;
	int		len;

	len = arraylen(*command_array);
	if (count == 3 && (*command_array)[index][0]
		!= (*command_array)[index + 1][0])
		return (0);
	if (index + count - 1 >= len)
		return (0);
	file = ft_strdup((*command_array)[index + count - 1]);
	if (!file || token->quoted[index + count - 1])
		return (0);
	if (!process_redirection_helper(token, file, count,
			&((*command_array)[index])))
		return (0);
	if (!remove_tokens_from_array(command_array, index, count))
	{
		free(token->redirs->file);
		free(token->redirs);
		token->redirs = NULL;
		return (0);
	}
	return (1);
}

static int	is_redir(char *s)
{
	if (s[0] == '\'' || s[0] == '\"')
		return (0);
	return (ft_strcmp(s, "<") == 0 || ft_strcmp(s, ">") == 0
		|| ft_strcmp(s, "<<") == 0 || ft_strcmp(s, ">>") == 0);
}

static int	handle_redir(char ***cmd, t_token *token, int *i, int *len)
{
	if (*i + 1 >= *len)
		return (0);
	if ((*cmd)[*i][0] == (*cmd)[*i + 1][0] && !token->quoted[*i + 1])
	{
		if (*i + 2 >= *len || token->quoted[*i + 2])
			return (0);
		if (!process_redirection(cmd, token, *i, 3))
			return (0);
	}
	else
	{
		if (*i + 1 >= *len || token->quoted[*i + 1])
			return (0);
		if (!process_redirection(cmd, token, *i, 2))
			return (0);
	}
	*len = arraylen(*cmd);
	*i -= 1;
	return (1);
}

int	cut_redirs(char **cmd, t_token *token)
{
	int	i;
	int	len;

	i = -1;
	len = arraylen(cmd);
	token->redirs = NULL;
	while (++i < len)
	{
		if (is_redir(cmd[i]) && !token->quoted[i])
		{
			if (i + 1 < len && token->quoted[i + 1])
				return (0);
			if (!handle_redir(&cmd, token, &i, &len))
				return (0);
		}
	}
	return (1);
}
