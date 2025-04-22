#include "minishell.h"

void	free_lexer(struct s_token **tokens, int i)
{
	// TODO
	// free insides too!
	while (i >= 0)
	{
		free(tokens[i--]);
	}
	free(tokens);
}

void free_char_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}
