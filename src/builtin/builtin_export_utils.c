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

char	**allocate_sorted_env(char **env, int count)
{
	char	**sorted;
	int		i;

	sorted = malloc((count + 1) * sizeof(char *));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted[i] = ft_strdup(env[i]);
		if (!sorted[i])
		{
			while (--i >= 0)
				free(sorted[i]);
			return (free(sorted), NULL);
		}
		i++;
	}
	sorted[count] = NULL;
	return (sorted);
}

void	bubble_sort_env(char **sorted, int count)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (env_var_cmp(sorted[j], sorted[j + 1]) > 0)
			{
				temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

char	**sort_env(char **env)
{
	char	**sorted;
	int		count;

	count = 0;
	while (env[count])
		count++;
	sorted = allocate_sorted_env(env, count);
	if (!sorted)
		return (NULL);
	bubble_sort_env(sorted, count);
	return (sorted);
}

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
