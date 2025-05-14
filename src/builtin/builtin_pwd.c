/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:50:28 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:50:29 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO remove printf
int	builtin_pwd(char **argv)
{
	char	*pwd;

	(void)argv;
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
