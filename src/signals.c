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

static volatile int	g_signal_received = 0;

void	ctrl_c_handler(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ctrl_backslash_handler(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_handler(int signalnumber)
{
	if (g_signal_received)
		return ;
	g_signal_received = 1;
	if (signalnumber == SIGINT)
		ctrl_c_handler();
	if (signalnumber == SIGQUIT)
		ctrl_backslash_handler();
	g_signal_received = 0;
}

// CHANGE sigaction gebruikt ipv signal.
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	memset(&sa_int, 0, sizeof(struct sigaction));
	memset(&sa_quit, 0, sizeof(struct sigaction));
	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction SIGINT");
		exit(1);
	}
	sa_quit.sa_handler = signal_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		exit(1);
	}
}
