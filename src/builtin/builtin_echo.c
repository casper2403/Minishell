/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:54 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:55 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	expanded_echo(char **argv, int last_exit, char **env, int i)
{
	char	*expanded;

	expanded = expand_variables(argv[i], last_exit, env);
	ft_putstr_fd(expanded, 1);
	update_last_command(&env, expanded);
	free(expanded);
}

int	builtin_echo(char **argv, bool *quoted, int last_exit, char **env)
{
	int	i;
	int	n_check;

	i = 0;
	n_check = 0;
	while (argv[++i] && ft_strcmp(argv[i], "-n") == 0)
		n_check = 1;
	while (argv[i])
	{
		if (quoted && quoted[i])
			ft_putstr_fd(argv[i], 1);
		else
			expanded_echo(argv, last_exit, env, i);
		if (argv[i + 1] && argv[i][0] != '\0')
			write(1, " ", 1);
		i++;
	}
	if (n_check == 0)
		write(1, "\n", 1);
	return (0);
}
