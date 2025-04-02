/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:20:08 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/12 17:32:41 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	put_d(int i);
int		len_d(int i);

int	print_d(int i)
{
	put_d(i);
	return (len_d(i));
}

void	put_d(int i)
{
	if (i == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (i < 0)
	{
		i = -i;
		write(1, "-", 1);
	}
	if (i >= 10)
	{
		put_d(i / 10);
		put_d(i % 10);
	}
	else
		print_c(i + '0');
}

int	len_d(int i)
{
	int	intlen;

	intlen = 0;
	if (i <= 0)
	{
		i = -i;
		intlen++;
	}
	while (i != 0)
	{
		i /= 10;
		intlen++;
	}
	return (intlen);
}
