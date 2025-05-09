/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:49:15 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:49:17 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_split
{
	int				i;
	int				j;
	int				start;
	bool			in_q[2];
	char			*temp;
	bool			single_q;
	char			**argv;
	bool			**quoted;
}					t_split;
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	bool			quoted;
	struct s_redir	*next;
}					t_redir;
typedef struct s_token
{
	char			**argv;
	bool			*quoted;
	bool			built_in;
	t_redir			*redirs;
	struct s_token	*next;
}					t_token;

typedef struct s_piper
{
	int				in_fd;
	int				pipe_fd[2];
	int				out_fd;
	pid_t			*pids;
	int				cmd_count;
	int				i;
}					t_piper;

// parser functions
int					process_input(char *input, int *last_exit, char ***env);
struct s_token		**parser(struct s_token **tokens);
struct s_token		**tokenizer(char *input);
int					is_builtin(const char *cmd);
// lexer functions
struct s_token		*lexer(char *split_by_pipe);
int					cut_redirs(char **command_array, struct s_token *token);
int					process_redirection(char ***command_array,
						struct s_token *token, int index, int count);
int					process_redirection_helper(struct s_token *token,
						char *file, int count, char **command_array);
int					remove_tokens_from_array(char ***command_array,
						int start_index, int count);
// redirection functions
void				add_redir(t_token *token, t_redir *new_redir);
t_redir				*create_redir(t_redir_type type, char *target);
t_redir_type		get_redir_type(char **cmd, int index);
// input processing functions
char				**split_input(char *input);
int					split_input_prepare(char *input, char ***tokenized);
int					split_input_process(char *input, char **tokenized,
						int *indices, bool *quotes);
int					count_pipes(char *input);
// argument functions
int					split_arguments(char *command, char **argv, bool **quoted);
int					split_arguments_process(char *command, char **argv,
						bool **quoted, int *indices);
int					split_arguments_handle_quotes(char *command, int *i,
						bool *in_quotes, bool *single_quoted);
int					split_arguments_handle_special(char *command, int *i,
						int *j, char **argv);
int					count_args(char *command);
// utility functions
int					arraylen(char **array);
void				free_lexer(t_token **tokens, int i);
void				free_char_array(char **array);
// executor
int					executor(t_token **tokens, int *last_exit, char ***env);
// built-ins
int					builtin_echo(char **argv, bool *quoted, int last_exit);
int					builtin_cd(char **argv, char ***env);
int					builtin_pwd(char **argv);
int					builtin_export(char **argv, char ***env);
int					builtin_unset(char **argv, char ***env);
int					builtin_env(char **argv, char ***env);
int					builtin_exit(char **argv);
// utils
void				append_str(char **dest, const char *src);
char				*expand_variables(char *str, int last_exit);
// executor utils
char				*get_var_name(char *str);
void				append_str(char **dest, const char *src);
void				append_char(char **dest, char c);
char				*extract_value(const char *s, int *len, int last_exit);
char				*expand_variables(char *str, int last_exit);
// export utils
int					exp_is_valid_var_name(char *name);
int					env_var_cmp(char *s1, char *s2);
char				**allocate_sorted_env(char **env, int count);
int					add_env_var(char *var, char ***env);
int					create_new_env_array(char *var, char ***env, int env_size);
int					update_existing_var(char *var, char ***env, int index);
int					find_env_var(char *var, char **env);
char				**sort_env(char **env);
void				bubble_sort_env(char **sorted, int count);
// split_arguments utils
void				handle_space(char *cmd, int *i, t_split *s);
void				handle_special_char(char *cmd, int *i, int *j, t_split *s);
// splitinput helper
void				handle_quotes(char c, bool *quotes);
// signals
void				ctrl_c_handler(void);
void				ctrl_backslash_handler(void);
void				signal_handler(int signalnumber);
void				setup_signals(void);
// cd utils
void				update_env_var(char ***env, char *var, char *value);
char				*get_target_path(char **argv);

#endif
