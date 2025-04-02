/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 11:20:55 by cstevens          #+#    #+#             */
/*   Updated: 2024/10/21 13:55:35 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

size_t	countnumbers(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count++;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*itoa;
	size_t	i;
	long	num;
	size_t	isneg;

	num = n;
	isneg = 0;
	i = countnumbers(n);
	itoa = (char *)malloc((i + 1) * sizeof(char));
	if (!itoa)
		return (NULL);
	if (n < 0)
	{
		num = -num;
		itoa[0] = '-';
		isneg = 1;
	}
	itoa[i] = '\0';
	while (i > isneg)
	{
		itoa[i - 1] = num % 10 + '0';
		num /= 10;
		i--;
	}
	return (itoa);
}
