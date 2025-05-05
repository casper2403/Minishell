/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:32:15 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/09 13:32:18 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Extract variable name from string */
char	*get_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

/* Append src to *dest, free old */
void	append_str(char **dest, const char *src)
{
	char	*old;

	old = *dest;
	*dest = ft_strjoin(old, src);
	free(old);
}

/* Append single char to *dest */
void	append_char(char **dest, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	append_str(dest, tmp);
}

/* Extract $VAR or $? value, set len */
char	*extract_value(const char *s, int *len, int last_exit)
{
	char	*var;
	char	*val;

	if (s[1] == '?')
	{
		*len = 2;
		return (ft_itoa(last_exit));
	}
	var = get_var_name((char *)s + 1);
	val = getenv(var);
	*len = ft_strlen(var) + 1;
	free(var);
	if (val)
		return (ft_strdup(val));
	else
		return (ft_strdup(""));
}

char	*expand_variables(char *str, int last_exit)
{
	char	*res;
	int		i;
	int		len;
	char	*v;

	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '?' || str[i + 1] == '_'))
		{
			v = extract_value(str + i, &len, last_exit);
			append_str(&res, v);
			free(v);
			i += len;
		}
		else
		{
			append_char(&res, str[i]);
			i++;
		}
	}
	return (res);
}
