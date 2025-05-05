/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:48:57 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:48:59 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = temp;
	}
}

void	free_lexer(struct s_token **tokens, int i)
{
	int		j;
	t_redir	*redir;
	t_redir	*next;

	while (i--)
	{
		j = 0;
		while (tokens[i]->argv && tokens[i]->argv[j])
			free(tokens[i]->argv[j++]);
		free(tokens[i]->argv);
		free(tokens[i]->quoted);
		redir = tokens[i]->redirs;
		while (redir)
		{
			next = redir->next;
			free(redir->file);
			free(redir);
			redir = next;
		}
		free(tokens[i]);
	}
	free(tokens);
}

void	free_char_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}
