/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:19:49 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 19:19:51 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_heredoc_interrupt = 0;

void	ctrl_c_handler(int i)
{
	(void)i;
	write(1, "\n", 1);
}

void	ctrl_c_other_handler(int i)
{
	(void)i;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ctrl_c_heredoc_handler(int i)
{
	(void)i;
	g_heredoc_interrupt = 1;
	close(0);
}

void	setup_signals(int mode)
{
	if (mode == 0)
	{
		signal(SIGINT, ctrl_c_other_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 1)
	{
		signal(SIGINT, ctrl_c_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 2)
	{
		signal(SIGINT, ctrl_c_heredoc_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
