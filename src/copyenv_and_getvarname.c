/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyenv_and_getvarname.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:46:00 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/16 16:46:02 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

char	**copy_env(char **env)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}
