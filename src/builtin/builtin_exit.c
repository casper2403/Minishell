/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:08 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:50:10 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, char **env, int *last_exit)
{
	static int	status = 0;

	(void)env;
	ft_putstr_fd("exit\n", 1);
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
			return (ft_putstr_fd("minishell: exit: ", 2), ft_putstr_fd(argv[1],
					2), ft_putstr_fd(": numeric argument required\n", 2), -2);
		status = ft_atoi(argv[1]);
	}
	if (status != 0)
	{
		if (status < 0)
			status = status + 256;
		return (status * -1);
	}
	if (*last_exit == 0)
		return (6969);
	return (*last_exit * -1);
}
