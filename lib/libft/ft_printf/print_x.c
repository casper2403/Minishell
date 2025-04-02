/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_x.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:21:06 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/12 17:52:28 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdint.h>

int	print_x(unsigned int ui)
{
	put_x(ui);
	return (len_x(ui));
}

void	put_x(unsigned long ui)
{
	if (ui >= 16)
	{
		put_x(ui / 16);
		put_x(ui % 16);
	}
	else
	{
		if (ui >= 10)
			print_c(ui - 10 + 'a');
		else
			print_c(ui + '0');
	}
}

int	len_x(unsigned long ul)
{
	int	count;

	count = 0;
	if (ul == 0)
		return (1);
	while (ul != 0)
	{
		ul /= 16;
		count++;
	}
	return (count);
}
