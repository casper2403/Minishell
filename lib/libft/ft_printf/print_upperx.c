/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_upperx.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:50:52 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/12 17:24:24 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	put_upperx(unsigned int ui);

int	print_upperx(unsigned int ui)
{
	put_upperx(ui);
	return (len_x(ui));
}

void	put_upperx(unsigned int ui)
{
	if (ui >= 16)
	{
		print_upperx(ui / 16);
		print_upperx(ui % 16);
	}
	else
	{
		if (ui >= 10)
			print_c(ui - 10 + 'A');
		else
			print_c(ui + '0');
	}
}
