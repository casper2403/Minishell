/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_p.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:47:45 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/26 14:33:25 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_p(void *p)
{
	int	ptrlen;

	if (p == NULL)
		return (write(1, "(nil)", 5));
	ptrlen = 0;
	ptrlen += write(1, "0x", 2);
	put_x((unsigned long)p);
	ptrlen += len_x((unsigned long)p);
	return (ptrlen);
}
