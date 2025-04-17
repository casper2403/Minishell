#include "minishell.h"

void	free_redirs(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = temp;
	}
}

void	free_lexer(struct s_token **tokens, int i)
{
	while (i >= 0)
	{
		if (tokens[i])
		{
			if (tokens[i]->argv)
				free_char_array(tokens[i]->argv);
			
			if (tokens[i]->redirs)
				free_redirs(tokens[i]->redirs);
			
			free(tokens[i]);
		}
		i--;
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

