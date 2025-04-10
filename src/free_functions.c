#include "minishell.h"

void free_lexer(struct s_token	**tokens, int i)
{
	// TODO
	// free insides too!
	while (i >= 0)
	{
		free(tokens[i--]);
	}
	free(tokens);
}
