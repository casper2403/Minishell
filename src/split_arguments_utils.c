/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arguments_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:54:07 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 15:54:15 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_special_char(char *cmd, int *i, int *j, t_split *s)
{
	char	*tmp;

	if (s->temp[0])
	{
		s->argv[*j] = s->temp;
		(*s->quoted)[(*j)++] = s->single_q;
		s->temp = ft_strdup("");
		s->single_q = false;
	}
	tmp = ft_substr(cmd, *i, 1);
	s->argv[*j] = tmp;
	(*s->quoted)[(*j)++] = false;
	(*i)++;
	s->start = *i;
}

void	handle_space(char *cmd, int *i, t_split *s)
{
	if (s->temp[0])
	{
		s->argv[s->j] = s->temp;
		(*s->quoted)[s->j++] = s->single_q;
		s->temp = ft_strdup("");
		s->single_q = false;
	}
	while (cmd[*i] == ' ')
		(*i)++;
	s->start = *i;
}
