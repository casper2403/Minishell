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

volatile sig_atomic_t g_heredoc_interrupt = 0;

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
    write(1, "\n", 1);
    close(0);
}

void	setup_signals(int i)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(struct sigaction));
	ft_memset(&sa_quit, 0, sizeof(struct sigaction));
	if (i == 1)
		sa_int.sa_handler = ctrl_c_handler;
	else if (i == 0)
		sa_int.sa_handler = ctrl_c_other_handler;
	else if (i == 2)
		sa_int.sa_handler = ctrl_c_heredoc_handler;
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
