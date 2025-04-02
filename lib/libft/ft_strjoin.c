/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:18:08 by cstevens          #+#    #+#             */
/*   Updated: 2024/10/17 19:30:13 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	tot_len;
	char	*strjoin;

	tot_len = ft_strlen(s1) + ft_strlen(s2);
	strjoin = (char *)malloc((tot_len + 1) * sizeof(char));
	if (!strjoin)
		return (NULL);
	ft_memcpy(strjoin, s1, ft_strlen(s1));
	ft_memcpy(&strjoin[ft_strlen(s1)], s2, ft_strlen(s2));
	strjoin[tot_len] = '\0';
	return (strjoin);
}
