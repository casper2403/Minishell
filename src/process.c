#include "minishell.h"

int lexer(char **div_input)
{
	return (0);
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
	
	tokenized = malloc(700 * sizeof(char *));
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
		i++;
	}
	tokenized[j++] = ft_substr(input, start, i - start);
	tokenized[j] = NULL;
	return (tokenized);
}

int tokenizer(char *input)
{
	int i = 0;
	char **divided_input;

	divided_input = split_input(input);
	if (!divided_input)
		return 1;
	while (divided_input[i])
	{
		if (lexer(divided_input))
			return (1);
		printf("%s\n", divided_input[i++]);
	}
	return (0);
}

// TOKENIZER + LEXER
// PARSER
int process_input(char *input)
{
	if (tokenizer(input))
		return (1);
	return (0);
}

// EXECUTOR
	// pid_t pid;
	//
	// printf("%s\n", getenv("PATH"));
	// pid = fork();
	// char *command[] = {"/usr/bin/ls", "-l" , "src", NULL};
	// if (pid == 0)
	// 	execve(command[0], command, NULL);
	// tokenizer();
	return (0);
}