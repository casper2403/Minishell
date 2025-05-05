/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:16 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:50:17 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**sort_env(char **env)
{
	char	**sorted;
	char	*temp;
	int		i;
	int		j;
	int		count;

	count = 0;
	while (env[count])
		count++;
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
	return (sorted);
}

static int	find_env_var(char *var, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static int	add_env_var(char *var, char ***env)
{
	int		index;
	int		i;
	int		j;
	char	**new_env;
	char	*new_var;

	index = find_env_var(var, *env);
	if (index >= 0)
	{
		new_var = ft_strdup(var);
		if (!new_var)
			return (1);
		free((*env)[index]);
		(*env)[index] = new_var;
		return (0);
	}
	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return (1);
	j = 0;
	while (j < i)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[i] = ft_strdup(var);
	if (!new_env[i])
		return (free(new_env), 1);
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

static void	print_export_list(char **env)
{
	char	**sorted;
	int		i;
	int		j;
	int		has_value;

	sorted = sort_env(env);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		j = 0;
		has_value = 0;
		while (sorted[i][j])
		{
			if (sorted[i][j] == '=')
			{
				has_value = 1;
				ft_putstr_fd("=\"", 1);
				j++;
				while (sorted[i][j])
				{
					if (sorted[i][j] == '\"' || sorted[i][j] == '\\' ||
						sorted[i][j] == '$')
						ft_putchar_fd('\\', 1);
					ft_putchar_fd(sorted[i][j], 1);
					j++;
				}
				ft_putchar_fd('\"', 1);
				break ;
			}
			ft_putchar_fd(sorted[i][j], 1);
			j++;
		}
		if (!has_value)
			ft_putchar_fd('\n', 1);
		else
			ft_putstr_fd("\n", 1);
		free(sorted[i]);
		i++;
	}
	free(sorted);
}

int	builtin_export(char **argv, char ***env)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (print_export_list(*env), 0);
	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (exp_is_valid_var_name(argv[i]))
		{
			if (add_env_var(argv[i], env))
				ret = 1;
		}
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		i++;
	}
	return (ret);
}
