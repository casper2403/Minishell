#include "minishell.h"
#include <stdio.h>

// split by spaces, keep track of quotes
// i think this is right
// look for edge cases
// goes into infinite loop when given < or >
int	count_args(char *command)
{
	bool	in_s_quote = false, in_d_quote = false;
	int		count = 0;
	int		i = 0;
	bool	in_word = false;

	in_s_quote = in_d_quote = i = count = in_word = 0;
	while (command[i])
	{
		if (!in_s_quote && !in_d_quote && (command[i] == '<' || command[i] == '>'))
		{
			in_word = false;
			count++;
			i++;
			continue;
		}
		if (!in_s_quote && !in_d_quote && command[i] == ' ')
		{
			in_word = false;
			i++;
			continue;
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
int	split_arguments(char *command, char **argv)
{
	bool	in_s_quote = false;
	bool	in_d_quote = false;
	int		i = 0;
	int		start = 0;
	int		j = 0;

	// printf("SPLIT TOKENS\n");
	while (command[i])
	{
		if (!in_s_quote && !in_d_quote && (command[i] == '<' || command[i] == '>'))
		{
			if (i > start)
			{
				argv[j++] = ft_substr(command, start, i - start);
				// printf("%s\n", argv[j - 1]);
			}
			argv[j++] = ft_substr(command, i, 1);
			// printf("%s\n", argv[j - 1]);
			i++;
			start = i;
			continue;
		}
		else if (!in_s_quote && !in_d_quote && command[i] == ' ')
		{
			if (i > start)
			{
				argv[j++] = ft_substr(command, start, i - start);
				// printf("%s\n", argv[j - 1]);
			}
			while (command[i] == ' ')
				i++;
			start = i;
			continue;
		}
		if (command[i] == '\'' && !in_d_quote)
			in_s_quote = !in_s_quote;
		else if (command[i] == '\"' && !in_s_quote)
			in_d_quote = !in_d_quote;
		i++;
	}
	if (i > start)
	{
		argv[j++] = ft_substr(command, start, i - start);
		// printf("%s\n", argv[j - 1]);
	}
	argv[j] = NULL;
	// printf("------------------------------------------------------");
	return (in_d_quote || in_s_quote);
}

int arraylen(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void redo_cmd_arr(char ***command_array, t_token *token, int index, int amount)
{
	char **new_command_array;
	int i;
	int j;

	i = 0;
	j = 0;
	new_command_array = malloc((arraylen(*command_array) - amount + 1) * sizeof(char *));
	while ((*command_array)[i])
	{
		if (i >= index && i < index + amount)
			;// printf("%s ", (*command_array)[i]);	// insert in redir struct here
		else
		{
			new_command_array[j++] = (*command_array)[i];
		}
		i++;
	}
	new_command_array[j] = NULL;
	free(*command_array);
	*command_array = new_command_array;
	// printf("\n");
}

// throw an error when incorrect < or >
void *cut_redirs(char **command_array, struct s_token *token)
{
	int i = 0;

	// printf("\nREDIRS\n");
	while (command_array[i])
	{
		if (command_array[i][0] == '>' || command_array[i][0] == '<')
		{
			if (!command_array[i + 1])
			{
				fprintf(stderr, "syntax error near unexpected token `newline`\n");
				return NULL;
			}
			if (command_array[i + 1][0] == command_array[i][0])
			{
				redo_cmd_arr(&command_array, token, i, 3);
			}
			else
			{
				redo_cmd_arr(&command_array, token, i, 2);
			}
			continue ;
		}
		i++;
	}
	// printf("------------------------------------------------------\n");
	// i = 0;
	// printf("COMMAND_ARRAY NO REDIRS\n");
	// while (command_array[i])
		// printf("%s\t", command_array[i++]);
	// printf("\n------------------------------------------------------\n\n\n\n");

	return NULL;
}

struct s_token	*lexer(char *split_by_pipe)
{
	struct s_token	*token;

	token = malloc(sizeof(struct s_token));
	if (!token)
		return (NULL);
	token->argv = malloc((count_args(split_by_pipe) + 1) * sizeof(char *));
	// printf("ARG_COUNT: %d\n", count_args(split_by_pipe));
	if(split_arguments(split_by_pipe, token->argv))
		return(write(1, "unclosed quotes\n", 16), NULL);
	cut_redirs(token->argv, token);

	// TODO
	// check all redirectors and keep rest as args
	// redirectors can be stuck to their args
	// make sure that everything is malloced
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
	in_s_quote = in_d_quote = false;
	i = 0, j = 0, start = 0;
	while (input[i])
	{
		if (input[i] == '|' && !in_s_quote && !in_d_quote)
		{
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
		for (int i = 0; divided_input[i]; i++) // TODO for loop norm
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

<<<<<<< HEAD
<<<<<<< HEAD
int	process_input(char *input, char ***local_env)
=======
=======

struct s_token **parser(struct s_token **tokens)
{
    // Free original tokens (to avoid leaks)
    if (tokens)
        free_lexer(tokens, count_pipes("test") + 1); // Simplified, adjust if needed

    // Allocate new tokens array (single token for testing)
    struct s_token **test_tokens = malloc(2 * sizeof(struct s_token *));
    if (!test_tokens)
        return NULL;

    // Create test token
    struct s_token *token = malloc(sizeof(struct s_token));
    if (!token)
    {
        free(test_tokens);
        return NULL;
    }

    // Set test token fields
    token->argv = malloc(3 * sizeof(char *));
    if (!token->argv)
    {
        free(token);
        free(test_tokens);
        return NULL;
    }
    token->argv[0] = ft_strdup("echo");
    token->argv[1] = ft_strdup("abcd");
    token->argv[2] = NULL;
    if (!token->argv[0] || !token->argv[1])
    {
        free(token->argv[0]);
        free(token->argv[1]);
        free(token->argv);
        free(token);
        free(test_tokens);
        return NULL;
    }
    token->redirs = NULL;
    token->built_in = true;
    token->next = NULL;

    // Set test_tokens
    test_tokens[0] = token;
    test_tokens[1] = NULL;

    return test_tokens;
}

>>>>>>> b0404b5 (fake parser and echo works)
int	process_input(char *input, int *last_exit, char ***env)
>>>>>>> 24866bc (added some stuff)
{
	struct s_token	**tokens;

	tokens = tokenizer(input);
	if (!tokens)
	{
		*last_exit = 1;
		return (1);
	}
	tokens = parser(tokens);
	
	executor(tokens, last_exit, env);
	return (0);
}

// TODO
// PARSER
	// check syntax

// TODO
// EXECUTOR
	// pid_t pid;
	//
	// printf("%s\n", getenv("PATH"));
	// pid = fork();
	// char *command[] = {"/usr/bin/ls", "-l" , "src", NULL};
	// if (pid == 0)
	// 	execve(command[0], command, NULL);
	// wait(NULL);
