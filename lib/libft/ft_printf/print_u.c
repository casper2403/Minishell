/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_u.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:50:51 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/07 17:22:08 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	put_u(unsigned int ui);
int		len_u(unsigned int ui);

int	print_u(unsigned int ui)
{
	put_u(ui);
	return (len_u(ui));
}

void	put_u(unsigned int ui)
{
	if (ui >= 10)
	{
		print_u(ui / 10);
		print_u(ui % 10);
	}
	else
		print_c(ui + '0');
}

int	len_u(unsigned int ui)
{
	int	uilen;

	uilen = 1;
	while (ui >= 10)
	{
		ui /= 10;
		uilen++;
	}
	return (uilen);
}
/*
#include <stdio.h>
int main(){
	printf("%i\n",len_u(223234));
	printf("%i\n",len_u(11));
	printf("%i\n",len_u(0));
	printf("%i\n",len_u(10));
}*/
