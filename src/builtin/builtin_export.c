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

static void	print_var_name(char *var)
{
	int	j;

	j = 0;
	while (var[j] && var[j] != '=')
	{
		ft_putchar_fd(var[j], 1);
		j++;
	}
}

static void	print_var_value(char *var, int j)
{
	ft_putstr_fd("=\"", 1);
	j++;
	while (var[j])
	{
		if (var[j] == '\"' || var[j] == '\\' || var[j] == '$')
			ft_putchar_fd('\\', 1);
		ft_putchar_fd(var[j], 1);
		j++;
	}
	ft_putchar_fd('\"', 1);
}

static void	print_one_var(char *var)
{
	int	j;
	int	has_value;

	j = 0;
	has_value = 0;
	print_var_name(var);
	while (var[j])
	{
		if (var[j] == '=')
		{
			has_value = 1;
			print_var_value(var, j);
			break ;
		}
		j++;
	}
	if (!has_value)
		ft_putchar_fd('\n', 1);
	else
		ft_putstr_fd("\n", 1);
}

static void	print_export_list(char **env)
{
	char	**sorted;
	int		i;

	sorted = sort_env(env);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		print_one_var(sorted[i]);
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
	update_last_command(env, "export");
	return (ret);
}
