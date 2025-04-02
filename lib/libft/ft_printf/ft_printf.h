/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:49:10 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/07 21:33:27 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int		ft_printf(const char *str, ...);
int		print_c(char c);
int		print_s(char *s);
int		print_p(void *p);
int		print_d(int i);
int		print_i(int i);
int		print_u(unsigned int ui);
int		print_x(unsigned int ui);

void	put_x(unsigned long ui);
int		len_x(unsigned long ul);

int		print_upperx(unsigned int ui);
int		print_percent(void);

#endif
