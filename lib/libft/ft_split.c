/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:40:28 by cstevens          #+#    #+#             */
/*   Updated: 2024/10/21 20:34:10 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

size_t		countwords(const char *str, char c);
static char	*fill_word(const char *str, int start, int end);
static void	*freeall(char **strs, int count);
static void	initvar(size_t *i, int *j, int *start);

char	**ft_split(const char *s, char c)
{
	char	**split;
	size_t	i;
	int		j;
	int		start;

	initvar(&i, &j, &start);
	split = ft_calloc((countwords(s, c) + 1), sizeof(char *));
	if (!split)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && start < 0)
			start = i;
		else if ((s[i] == c || i == ft_strlen(s)) && start >= 0)
		{
			split[j] = fill_word(s, start, i);
			if (!(split[j]))
				return (freeall(split, j));
			start = -1;
			j++;
		}
		i++;
	}
	return (split);
}

static void	initvar(size_t *i, int *j, int *start)
{
	*i = 0;
	*j = 0;
	*start = -1;
}

static void	*freeall(char **strs, int count)
{
	while (count >= 0)
	{
		free(strs[count--]);
	}
	free(strs);
	return (NULL);
}

static char	*fill_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = 0;
	return (word);
}

size_t	countwords(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			count++;
			while (*s != c && *s)
				s++;
		}
	}
	return (count);
}
