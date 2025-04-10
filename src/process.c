#include "minishell.h"

int count_args(char *command)
{
	return 
}



// split by spaces except for in quotes
int split_arguments(char *command, char **argv)
{
	bool in_s_quote;
	bool in_d_quote;
	int i;
	int start;
	int j;

	in_s_quote = in_d_quote	= i = j = start = 0;
	while (command[i])
	{
		if (command[i] == ' ' && !in_s_quote && !in_d_quote)
		{
			argv[j++] = ft_substr(command, start, i - start);
			start = i + 1;
		}
		if (command[i] == '\'')
			in_s_quote = !in_s_quote;
		if (command[i] == '\"')
			in_d_quote = !in_d_quote;
		i++;
	}
	if (in_d_quote == true || in_s_quote == true)
		return (1); // unclosed quotes. should not be handled
	return (0);
}

struct s_token *lexer(char *split_by_pipe)
{
	struct s_token *token;

	token = malloc(sizeof(struct s_token));
	if (!token)
		return NULL;
	token->argv = malloc((count_args(split_by_pipe) + 1) *sizeof(char *));
	split_arguments(split_by_pipe, token->argv);
	// TODO
	// check all redirectors and keep rest as args
	// make sure that everything is malloced
	return (token);
}

int count_pipes(char *input)
{
	int i = 0;
	int count = 0;

	while (input[i])
	{
		if (input[i++] == '|')
			count++;
	}
	return (count);
}

// split by pipes, IF the pipe is not in quotes!!
char **split_input(char *input)
{
	char **tokenized;
	bool in_s_quote;
	bool in_d_quote;
	int i;
	int start;
	int j;
	
	tokenized = malloc((count_pipes(input) + 2) * sizeof(char *));
	if (!tokenized)
		return (NULL);
	in_s_quote = in_d_quote	= i = j = start = 0;
	while(input[i])
	{
		if (input[i] == '|' && !in_s_quote && !in_d_quote)
		{
			tokenized[j++] = ft_substr(input, start, i - start);
			start = i + 1;
		}
		if (input[i] == '\'')
			in_s_quote = !in_s_quote;
		if (input[i] == '\"')
			in_d_quote = !in_d_quote;
		if (input[i] == ';' || input[i] == '\\')
		{
			while (j > 0)
				free(tokenized[--j]);
			free(tokenized);
			return (NULL);
		}
		i++;
	}
	tokenized[j++] = ft_substr(input, start, i - start);
	tokenized[j] = NULL;
	return (tokenized);
}

struct s_token **tokenizer(char *input)
{
	int				i = 0;
	char			**divided_input;
	struct s_token	**tokens;
	
	divided_input = split_input(input);
	tokens = malloc((count_pipes(input) + 2 ) * sizeof(struct s_token *));
	if (!divided_input)
		return NULL;
	while (divided_input[i])
	{
		tokens[i] = lexer(divided_input[i]);
		if (!tokens[i])
		{
			free_lexer(tokens, i);
			return (NULL);
		}
		printf("%s\n", divided_input[i]);
		free(divided_input[i++]);
	}
	free(divided_input);
	return (tokens);
}

int process_input(char *input)
{
	if (!tokenizer(input))
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
