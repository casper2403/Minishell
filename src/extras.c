/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvandebe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:07:28 by pvandebe          #+#    #+#             */
/*   Updated: 2025/05/21 14:07:34 by pvandebe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

const char	*get_last_arg(struct s_token *tok)
{
	int	i;

	i = 0;
	if (!tok->argv || !tok->argv[0])
		return (NULL);
	while (tok->argv[i + 1])
		i++;
	return (tok->argv[i]);
}

int	return_right_exit_value(int last_exit)
{
	if (last_exit == 6969)
		return (0);
	else if (last_exit < 0)
		last_exit = last_exit * -1;
	else if (last_exit != 6969 && last_exit > 0)
		last_exit = (last_exit * -1) + 256;
	return (last_exit);
}
