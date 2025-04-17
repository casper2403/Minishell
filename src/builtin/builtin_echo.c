#include "../minishell.h"


//TODO should call expand_variables to check for $
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
	char *expanded;

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
			expanded = expand_variables(argv[i], 0); // last_exit not directly available
        	ft_putstr_fd(expanded, 1);
        	free(expanded);
			if (argv[i + 1] && argv[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (n_check == 0)
		write(1, "\n", 1);
	return (0);
}
