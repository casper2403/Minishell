/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:47:06 by cstevens          #+#    #+#             */
/*   Updated: 2024/10/16 15:02:45 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	i = 0;
	if (d < s)
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	if (d > s)
	{
		while (n > 0)
		{
			d[n - 1] = s[n - 1];
			n--;
		}
	}
	return (d);
}
/*
#include <stdio.h>

int	main(void) {
	char src[] = "HelloWorld";
	char dst1[11];
	char dst2[11];

	// Using the standard memmove
	memmove(dst1, src, 11);
	dst1[10] = '\0';
	printf("Standard memmove: %s\n", dst1);

	// Using your ft_memmove
	ft_memmove(dst2, src, 11);
	dst2[10] = '\0';
	printf("ft_memmove: %s\n", dst2);

<<<<<<< HEAD
    return 0;
=======
	// Compare results
	if (strcmp(dst1, dst2) == 0) {
		printf("The functions produce the same result.\n");
	} else {
		printf("The functions produce different results.\n");
	}

	return (0);
>>>>>>> refs/remotes/origin/main
}*/
