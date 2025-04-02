/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:00:28 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/28 15:00:34 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_isinstr(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	return (0);
}

char	*ft_gnl_strjoin(char *s1, char *s2)
{
	size_t	tot_len;
	char	*strjoin;
	size_t	j;

	if (!s1)
		s1 = ft_gnl_strdup("");
	if (!s2)
		s2 = ft_gnl_strdup("");
	tot_len = ft_gnl_strlen(s1) + ft_gnl_strlen(s2);
	strjoin = malloc((tot_len + 1) * sizeof(char));
	if (!strjoin)
		return (NULL);
	tot_len = -1;
	j = -1;
	while (s1[++tot_len])
		strjoin[tot_len] = s1[tot_len];
	while (s2[++j])
		strjoin[tot_len + j] = s2[j];
	strjoin[tot_len + j] = '\0';
	free(s1);
	free(s2);
	return (strjoin);
}

size_t	ft_gnl_strlen(const char *s)
{
	size_t	size;

	size = 0;
	while (*s != '\0')
	{
		size++;
		s++;
	}
	return (size);
}

char	*ft_gnl_strdup(const char *s)
{
	size_t	len;
	char	*new;
	size_t	i;

	i = 0;
	len = ft_gnl_strlen(s);
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	while (i <= len)
	{
		new[i] = s[i];
		i++;
	}
	return (new);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_gnl_strlen(src);
	if (!size)
		return (src_len);
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}
