/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:15:10 by cstevens          #+#    #+#             */
/*   Updated: 2024/10/17 18:34:18 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*strdup;
	size_t	len;

	len = ft_strlen(s) + 1;
	strdup = (char *)malloc(len * sizeof(char));
	if (!strdup)
		return (NULL);
	ft_memcpy(strdup, s, len);
	return (strdup);
}
