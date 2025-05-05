/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:23 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(char *cmd, int *i, bool in_q[2], bool *single_q)
{
	if (cmd[*i] == '\'' && !in_q[1])
	{
		in_q[0] = !in_q[0];
		if (in_q[0])
			*single_q = true;
		(*i)++;
		return (1);
	}
	else if (cmd[*i] == '\"' && !in_q[0])
	{
		in_q[1] = !in_q[1];
		(*i)++;
		return (1);
	}
	return (0);
}

static void	add_arg(char **temp, char **argv, bool **quoted, int *j)
{
	argv[*j] = *temp;
	(*quoted)[(*j)++] = true;
	*temp = ft_strdup("");
}

static int	process_loop(char *cmd, t_split *s)
{
	int	i;

	i = s->i;
	while (cmd[i])
	{
		if (!s->in_q[0] && !s->in_q[1] && (cmd[i] == '<' || cmd[i] == '>'))
		{
			handle_special_char(cmd, &i, &s->j, s);
			continue ;
		}
		else if (!s->in_q[0] && !s->in_q[1] && cmd[i] == ' ')
		{
			handle_space(cmd, &i, s);
			continue ;
		}
		if (handle_quotes(cmd, &i, s->in_q, &s->single_q))
			continue ;
		if (i >= s->start)
			append_char(&s->temp, cmd[i]);
		i++;
	}
	s->i = i;
	return (0);
}

static int	finalize_args(t_split *s)
{
	if (s->temp[0])
	{
		s->argv[s->j] = s->temp;
		(*s->quoted)[s->j++] = s->single_q;
	}
	else
		free(s->temp);
	s->argv[s->j] = NULL;
	(*s->quoted)[s->j] = false;
	return (s->in_q[0] || s->in_q[1]);
}

int	split_arguments(char *cmd, char **argv, bool **quoted)
{
	t_split	s;

	s.i = 0;
	s.j = 0;
	s.start = 0;
	s.in_q[0] = false;
	s.in_q[1] = false;
	s.temp = ft_strdup("");
	s.single_q = false;
	s.argv = argv;
	s.quoted = quoted;
	*quoted = malloc((count_args(cmd) + 1) * sizeof(bool));
	if (!*quoted)
		return (1);
	process_loop(cmd, &s);
	return (finalize_args(&s));
}
