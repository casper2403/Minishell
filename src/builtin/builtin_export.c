#include "../minishell.h"

static int	is_valid_var_name(char *name)
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

static int	env_var_cmp(char *s1, char *s2)
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
			free(sorted);
			return (NULL);
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
		if (ft_strncmp(env[i], var, len) == 0 && 
			(env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static int	add_env_var(char *var, char ***env)
{
	int		index;
	int		i;
	char	**new_env;
	
	index = find_env_var(var, *env);
	if (index >= 0)
	{
		char *new_var = ft_strdup(var);
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
	for (int j = 0; j < i; j++)
		new_env[j] = (*env)[j];
	new_env[i] = ft_strdup(var);
	if (!new_env[i])
	{
		free(new_env);
		return (1);
	}
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
		return;
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
				break;
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
	{
		print_export_list(*env);
		return (0);
	}
	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (is_valid_var_name(argv[i]))
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
