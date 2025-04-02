/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:58:46 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/26 14:33:45 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

int	format(va_list args, const char format);

int	ft_printf(const char *str, ...)
{
	int		strlen;
	int		i;
	va_list	args;

	strlen = 0;
	i = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			strlen += format(args, str[i + 1]);
			i++;
		}
		else
			strlen += print_c(str[i]);
		i++;
	}
	va_end(args);
	return (strlen);
}

int	format(va_list args, const char format)
{
	int	arglen;

	arglen = 0;
	if (format == 'c')
		arglen += print_c(va_arg(args, int));
	else if (format == 's')
		arglen += print_s(va_arg(args, char *));
	else if (format == 'p')
		arglen += print_p(va_arg(args, void *));
	else if (format == 'd')
		arglen += print_d(va_arg(args, int));
	else if (format == 'i')
		arglen += print_i(va_arg(args, int));
	else if (format == 'u')
		arglen += print_u(va_arg(args, unsigned int));
	else if (format == 'x')
		arglen += print_x(va_arg(args, unsigned int));
	else if (format == 'X')
		arglen += print_upperx(va_arg(args, unsigned int));
	else if (format == '%')
		arglen += print_percent();
	return (arglen);
}
