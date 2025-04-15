#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char *file;  // or delimiter if heredoc
	bool quoted; // for heredoc (<< vs <<'EOF') no quotes will expand
	struct s_redir	*next;
}					t_redir;

typedef struct s_token
{
	char **argv;     // command + args
	bool built_in;   // is the command a built-in
	t_redir *redirs; // list of redirections
	struct s_token	*next;
}					t_token;

int					process_input(char *input, int *last_exit, char ***env);
void				free_lexer(struct s_token **tokens, int i);


//executor 
int executor(struct s_token **tokens, int *last_exit, char ***env);

//buildin
int builtin_echo(char **argv);
int builtin_cd(char **argv, char ***env);
int builtin_pwd(char **argv);
int builtin_export(char **argv, char ***env);
int builtin_unset(char **argv, char ***env);
int builtin_env(char **argv, char ***env);
int builtin_exit(char **argv);


#endif
