#include "minishell.h"

static	int		args_counter(char **argv)
{
	int		count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

int				builtin_echo(char **argv)
{
	int		i;
	int		n_check;

	i = 1;
	n_check = 0;
	if (args_counter(argv) > 1)
	{
		while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
		{
			n_check = 1;
			i++;
		}
		while (argv[i])
		{
			ft_putstr_fd(argv[i], 1);
			if (argv[i + 1] && argv[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (n_check == 0)
		write(1, "\n", 1);
	return (0);
}