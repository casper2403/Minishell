#include "minishell.h"

// split by spaces, keep track of quotes
// i think this is right
// look for edge cases
int	count_args(char *command)
{
	bool	in_s_quote = false, in_d_quote;
	int		count;
	int		i;
	bool	in_word;

	in_s_quote = false, in_d_quote = false;
	count = 0;
	i = 0;
	in_word = false;
	while (command[i])
	{
		if ((command[i] == ' ' || command[i] == '<' || command[i] == '>')
			&& !in_s_quote && !in_d_quote)
		{
			in_word = false;
			while (command[i] == ' ')
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
int	split_arguments(char *command, char **argv)
{
	bool	in_s_quote = false, in_d_quote;
	int		i = 0, start = 0, j;

	in_s_quote = false, in_d_quote = false;
	i = 0, start = 0, j = 0;
	while (command[i])
	{
		if ((command[i] == ' ' || command[i] == '<' || command[i] == '>')
			&& !in_s_quote && !in_d_quote)
		{
			if (i > start)
			{
				argv[j++] = ft_substr(command, start, i - start);
				printf("%s\n", argv[j - 1]);
			}
			while (command[i] == ' ')
				i++;
			start = i;
			continue ;
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
		printf("%s\n", argv[j - 1]);
	}
	argv[j] = NULL;
	return (in_d_quote || in_s_quote);
}

struct s_token	*lexer(char *split_by_pipe)
{
	struct s_token	*token;

	token = malloc(sizeof(struct s_token));
	if (!token)
		return (NULL);
	token->argv = malloc((count_args(split_by_pipe) + 1) * sizeof(char *));
	split_arguments(split_by_pipe, token->argv);
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
			tokenized[j++] = ft_substr(input, start, i - start); // segfault
			if (!tokenized[j - 1])
			{
				while (j > 0)
					free(tokenized[--j]);
				free(tokenized);
				return (NULL);
			}
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
		while (j > 0)
			free(tokenized[--j]);
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

int	process_input(char *input, char ***local_env)
{
	struct s_token	**tokens;

	tokens = tokenizer(input);
	if (!tokens)
		return (1);
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
