#include "minishell.h"

int execute_builtin(struct s_token *token, int *last_exit, char ***env) {
    if (ft_strcmp(token->argv[0], "echo") == 0)
        return builtin_echo(token->argv);
    if (ft_strcmp(token->argv[0], "cd") == 0)
        return builtin_cd(token->argv, env);
    if (ft_strcmp(token->argv[0], "pwd") == 0)
        return builtin_pwd(token->argv);
    if (ft_strcmp(token->argv[0], "export") == 0)
        return builtin_export(token->argv, env);
    if (ft_strcmp(token->argv[0], "unset") == 0)
        return builtin_unset(token->argv, env);
    if (ft_strcmp(token->argv[0], "env") == 0)
        return builtin_env(token->argv, env);
    if (ft_strcmp(token->argv[0], "exit") == 0)
        builtin_exit(token->argv);
    return (0);
}

int execute_command(struct s_token *token, int *last_exit, int in_fd, int out_fd, char **env) {
    pid_t pid;
    if (token->built_in && !token->redirs)
        return execute_builtin(token, last_exit, &env);

    //TODO

    return (0);
}


int executor(struct s_token **tokens, int *last_exit, char ***env) 
{
    int pipe_fd[2];
    int in_fd;
    int out_fd;
    int i;

    in_fd = STDERR_FILENO;
    i = 0;
    while(tokens[i])
    {
        out_fd = STDOUT_FILENO;
        if (tokens[i + 1])
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return (1);
            }
            out_fd = pipe_fd[1];
        }
        execute_command(tokens[i], last_exit, in_fd, out_fd, *env);
        if (tokens[i + 1])
            in_fd = pipe_fd[0];
        i++;
    }
    return (*last_exit);
}