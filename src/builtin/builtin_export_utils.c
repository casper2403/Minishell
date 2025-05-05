/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:35:18 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 13:35:31 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_var_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && s1[i] != '=' && s2[i] != '=')
		i++;
	if ((s1[i] == '=' || !s1[i]) && (s2[i] == '=' || !s2[i]))
		return (0);
	if (s1[i] == '=' || !s1[i])
		return (-1);
	if (s2[i] == '=' || !s2[i])
		return (1);
	return (s1[i] - s2[i]);
}

int	exp_is_valid_var_name(char *name)
{
	int	i;
	int	equal_pos;

	if (!name || !*name)
		return (0);
	equal_pos = 0;
	while (name[equal_pos] && name[equal_pos] != '=')
		equal_pos++;
	if (equal_pos == 0 || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (i < equal_pos)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
