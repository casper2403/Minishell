#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "../lib/libft/libft.h"

typedef enum e_redir_type {
    REDIR_IN,     // <
    REDIR_OUT,    // >
    REDIR_APPEND, // >>
    REDIR_HEREDOC // <<
} t_redir_type;

typedef struct s_redir {
    t_redir_type type;
    char *file;             // or delimiter if heredoc
    bool quoted;            // for heredoc (<< vs <<'EOF') no quotes will expand
    struct s_redir *next;
} t_redir;

typedef struct s_token
{
	char **argv;               // command + args
	bool built_in;			   // is the command a built-in
	t_redir *redirs;           // list of redirections
	struct s_token *next;
} t_token;

int process_input(char *input);
void free_lexer(struct s_token	**tokens, int i);

#endif
