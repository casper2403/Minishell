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
