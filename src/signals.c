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

void	ctrl_c_handler(void)
{
	write(1, "\n", 1);
	if (!g_is_child_running)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ctrl_backslash_handler(void)
{
}

void	signal_handler(int signalnumber)
{
	if (signalnumber == SIGINT)
		ctrl_c_handler();
	if (signalnumber == SIGQUIT)
		ctrl_backslash_handler();
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(struct sigaction));
	ft_memset(&sa_quit, 0, sizeof(struct sigaction));
	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction SIGINT");
		exit(1);
	}
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		exit(1);
	}
}
