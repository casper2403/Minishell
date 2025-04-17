#include "minishell.h"
#include <stdbool.h>

// split by spaces, keep track of quotes
// i think this is right
// look for edge cases
// goes into infinite loop when given < or >
int	count_args(char *command)
{
	bool	in_s_quote;
	bool	in_d_quote;
	int		count;
	int		i;
	bool	in_word;

	in_s_quote = false;
	in_d_quote = false;
	i = 0;
	count = 0;
	in_word = false;
	while (command[i])
	{
		if (!in_s_quote && !in_d_quote
			&& (command[i] == '<' || command[i] == '>'))
		{
			in_word = false;
			count++;
			i++;
			continue ;
		}
		if (!in_s_quote && !in_d_quote && command[i] == ' ')
		{
			in_word = false;
			i++;
			continue ;
		}
		if (!in_word)
		{
			count++;
			in_word = true;
		}
		if (command[i] == '\'' && !in_d_quote)
			in_s_quote = !in_s_quote;
		else if (command[i] == '\"' && !in_s_quote)
			in_d_quote = !in_d_quote;
		i++;
	}
	return (count);
}

// split by spaces except for in quotes
int split_arguments(char *command, char **argv)
{
    bool in_s_quote = false;
    bool in_d_quote = false;
    int i = 0;
    int start = 0;
    int j = 0;
    char quote = 0;

    while (command[i])
    {
        if (!in_s_quote && !in_d_quote && (command[i] == '<' || command[i] == '>'))
        {
            if (i > start)
                argv[j++] = ft_substr(command, start, i - start);
            argv[j++] = ft_substr(command, i, 1);
            i++;
            start = i;
            continue;
        }
        else if (!in_s_quote && !in_d_quote && command[i] == ' ')
        {
            if (i > start)
                argv[j++] = ft_substr(command, start, i - start);
            while (command[i] == ' ')
                i++;
            start = i;
            continue;
        }
        if (command[i] == '\'' && !in_d_quote && !in_s_quote)
        {
            in_s_quote = true;
            quote = '\'';
            i++;
            start = i;
            continue;
        }
        else if (command[i] == '\"' && !in_s_quote && !in_d_quote)
        {
            in_d_quote = true;
            quote = '\"';
            i++;
            start = i;
            continue;
        }
        else if ((command[i] == '\'' && in_s_quote) || (command[i] == '\"' && in_d_quote))
        {
            argv[j++] = ft_substr(command, start, i - start);
            in_s_quote = false;
            in_d_quote = false;
            quote = 0;
            i++;
            start = i;
            continue;
        }
        i++;
    }
    if (i > start)
        argv[j++] = ft_substr(command, start, i - start);
    argv[j] = NULL;
    return (in_d_quote || in_s_quote);
}

int	arraylen(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

static t_redir_type	get_redir_type(char **cmd, int index)
{
	if (cmd[index][0] == '<')
	{
		if (cmd[index + 1] && cmd[index + 1][0] == '<')
			return (REDIR_HEREDOC);
		return (REDIR_IN);
	}
	if (cmd[index][0] == '>')
	{
		if (cmd[index + 1] && cmd[index + 1][0] == '>')
			return (REDIR_APPEND);
		return (REDIR_OUT);
	}
	return (REDIR_OUT);
}

// TODO detect quote on target if needed
static t_redir	*create_redir(t_redir_type type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = strdup(target);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->quoted = false;
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_token *token, t_redir *new_redir)
{
	t_redir	*curr;

	if (!new_redir)
		return ;
	if (!token->redirs)
		token->redirs = new_redir;
	else
	{
		curr = token->redirs;
		while (curr->next)
			curr = curr->next;
		curr->next = new_redir;
	}
}

int	remove_tokens_from_array(char ***command_array, int start_index, int count)
{
	int		i;
	int		len;
	char	**array;
	int		j;

	i = 0;
	j = 0;
	len = 0;
	array = *command_array;
	if (!array)
		return (0);
	while (array[len])
		len++;
	if (start_index < 0 || start_index + count > len)
		return (0);
	while (i < count)
	{
		if (array[start_index + i])
			free(array[start_index + i]);
		i++;
	}
	i = start_index;
	j = start_index + count;
	while (j < len)
	{
		array[i] = array[j];
		i++;
		j++;
	}
	while (i < len)
	{
		array[i] = NULL;
		i++;
	}
	return (1);
}

int	process_redirection(char ***command_array,
						t_token *token, int index, int tokens_to_use)
{
	t_redir_type	type;
	t_redir			*redir;

	type = get_redir_type(*command_array, index);
	redir = create_redir(type, (*command_array)[index + tokens_to_use - 1]);
	if (!redir)
		return 0;
	add_redir(token, redir);
	return remove_tokens_from_array(command_array, index, tokens_to_use);
}

int	cut_redirs(char **command_array, struct s_token *token)
{
	int i;
	int len;

	i = 0;
	len = 0;
	token->redirs = NULL;
	while (command_array[len])
		len++;
	i = 0;
	while (i < len) {
		if (command_array[i][0] == '>' || command_array[i][0] == '<')
		{
			if (i + 1 >= len)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline`\n", 2);
				return 0;
			}
			if (i + 1 < len && command_array[i + 1][0] == command_array[i][0]) {
				if (i + 2 >= len)
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `newline`\n", 2);
					return 0;
				}
				if (!process_redirection(&command_array, token, i, 3))
					return 0;
			}
			else
		{
				if (!process_redirection(&command_array, token, i, 2))
					return 0;
			}
			len = 0;
			while (command_array[len])
				len++;
		}
		else
		i++;
	}

	return 1;
}

struct s_token	*lexer(char *split_by_pipe)
{
	struct s_token	*token;

	token = malloc(sizeof(struct s_token));
	if (!token)
		return (NULL);
	token->redirs = NULL;
	token->next = NULL;
	token->built_in = false;
	token->argv = malloc((count_args(split_by_pipe) + 1) * sizeof(char *));
	if (!token->argv)
	{
		free(token);
		return (NULL);
	}
	if (split_arguments(split_by_pipe, token->argv))
	{
		write(1, "unclosed quotes\n", 16);
		free(token->argv);
		free(token);
		return NULL;
	}
	cut_redirs(token->argv, token);
	return (token);
}

// TODO
// incorrect! should keep track of quotes!
// check if full fixed
int	count_pipes(char *input)
{
	int		i;
	int		count;
	bool	in_s_quote;
	bool	in_d_quote;

	in_s_quote = in_d_quote = i = count = 0;
	while (input[i])
	{
		if (input[i] == '|' && !in_s_quote && !in_d_quote)
			count++;
		if (input[i] == '\'' && !in_d_quote)
			in_s_quote = !in_s_quote;
		if (input[i] == '\"' && !in_s_quote)
			in_d_quote = !in_d_quote;
		i++;
	}
	return (count);
}

// split by pipes, IF the pipe is not in quotes!!
char	**split_input(char *input)
{
	char	**tokenized;
	bool	in_s_quote;
	bool	in_d_quote;
	int		i;
	int		start;
	int		j;

	tokenized = malloc((count_pipes(input) + 2) * sizeof(char *));
	if (!tokenized)
		return (NULL);
	in_s_quote = false;
	in_d_quote = false;
	i = 0;
	j = 0;
	start = 0;
	while (input[i])
	{
		if (input[i] == '|' && !in_s_quote && !in_d_quote)
		{
			tokenized[j++] = ft_substr(input, start, i - start);
			tokenized[j++] = ft_substr(input, start, i - start);
			if (!tokenized[j - 1])
			{
				while (--j >= 0)
					free(tokenized[j]);
				free(tokenized);
				return (NULL);
			}
			while (input[i] == ' ')
				i++;
			start = i + 1;
		}
		else if (input[i] == ';' || input[i] == '\\')
		{
			while (j > 0)
				free(tokenized[--j]);
			free(tokenized);
			return (NULL);
		}
		else
	{
			if (input[i] == '\'' && !in_d_quote)
				in_s_quote = !in_s_quote;
			if (input[i] == '\"' && !in_s_quote)
				in_d_quote = !in_d_quote;
		}
		i++;
	}
	tokenized[j++] = ft_substr(input, start, i - start);
	if (!tokenized[j - 1])
	{
		while (--j >= 0)
			free(tokenized[j]);
		free(tokenized);
		return (NULL);
	}
	tokenized[j] = NULL;
	return (tokenized);
}

// CHANGE extra loops toegevoegd voor de memory cleanup.
// TODO for loop norm
struct s_token	**tokenizer(char *input)
{
	int				i;
	char			**divided_input;
	struct s_token	**tokens;

	i = 0;
	divided_input = split_input(input);
	if (!divided_input)
		return (NULL);
	tokens = malloc((count_pipes(input) + 2) * sizeof(struct s_token *));
	if (!tokens)
	{
		for (int i = 0; divided_input[i]; i++)
			free(divided_input[i]);
		free(divided_input);
		return (NULL);
	}
	while (divided_input[i])
	{
		tokens[i] = lexer(divided_input[i]);
		if (!tokens[i])
		{
			free_lexer(tokens, i);
			for (int j = 0; divided_input[j]; j++)
				free(divided_input[j]);
			free(divided_input);
			return (NULL);
		}
		free(divided_input[i++]);
	}
	free(divided_input);
	tokens[i] = NULL;
	return (tokens);
}

struct s_token **parser(struct s_token **tokens)
{
	int		i;
	t_redir	*redir;

	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->argv && tokens[i]->argv[0])
		{
			if (ft_strcmp(tokens[i]->argv[0], "echo") == 0
				|| ft_strcmp(tokens[i]->argv[0], "cd") == 0
				|| ft_strcmp(tokens[i]->argv[0], "pwd") == 0
				|| ft_strcmp(tokens[i]->argv[0], "export") == 0
				|| ft_strcmp(tokens[i]->argv[0], "unset") == 0
				|| ft_strcmp(tokens[i]->argv[0], "env") == 0
				|| ft_strcmp(tokens[i]->argv[0], "exit") == 0)
				tokens[i]->built_in = true;
			else
				tokens[i]->built_in = false;
		}
		redir = tokens[i]->redirs;
		while (redir)
		{
			if (!redir->file || redir->file[0] == '\0')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
				free_lexer(tokens, i);
				return (NULL);
			}
			if (redir->type == REDIR_HEREDOC)
			{
				if (ft_strchr(redir->file, ' ') || ft_strchr(redir->file, '\t'))
				{
					ft_putstr_fd("minishell: syntax error in heredoc delimiter\n", 2);
					free_lexer(tokens, i);
					return (NULL);
				}
				if (redir->file[0] == '\'' || redir->file[0] == '"')
					redir->quoted = true;
			}
			redir = redir->next;
		}
		i++;
	}
	return (tokens);
}

// TODO: Add cleanup for tokens after execution
int	process_input(char *input, int *last_exit, char ***env)

{
	struct s_token	**tokens;

	tokens = tokenizer(input);
	if (!tokens)
	{
		*last_exit = 1;
		return (1);
	}
	tokens = parser(tokens);
	if (!tokens)
	{
		*last_exit = 1;
		return (1);
	}
	executor(tokens, last_exit, env);
	return (0);
}
