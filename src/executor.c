/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:48:13 by cstevens          #+#    #+#             */
/*   Updated: 2025/05/08 10:48:18 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(struct s_token *token, int *last_exit, char ***env)
{
	if (!token || !token->argv || !token->argv[0])
		return (*last_exit = 1);
	if (ft_strcmp(token->argv[0], "echo") == 0)
		return (*last_exit = builtin_echo(token->argv,
				token->quoted, *last_exit));
	if (ft_strcmp(token->argv[0], "cd") == 0)
		return (*last_exit = builtin_cd(token->argv, env));
	if (ft_strcmp(token->argv[0], "pwd") == 0)
		return (*last_exit = builtin_pwd(token->argv));
	if (ft_strcmp(token->argv[0], "export") == 0)
		return (*last_exit = builtin_export(token->argv, env));
	if (ft_strcmp(token->argv[0], "unset") == 0)
		return (*last_exit = builtin_unset(token->argv, env));
	if (ft_strcmp(token->argv[0], "env") == 0)
		return (*last_exit = builtin_env(token->argv, env));
	if (ft_strcmp(token->argv[0], "exit") == 0)
		return (builtin_exit(token->argv));
	return (*last_exit = 0);
}

/* Search cmd in dirs array */
static char	*search_in_dirs(char *cmd, char **dirs)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*res;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		return (ft_strdup(cmd));
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	res = search_in_dirs(cmd, dirs);
	i = 0;
	while (dirs[i])
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
	return (res);
}

/* Heredoc: read until delimiter */
static int	handle_heredoc(t_redir *r, int last_exit)
{
	int		pipe_fd[2];
	char	*line;
	char	*exp;
	int		fd;

	pipe(pipe_fd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, r->file) == 0)
		{
			free(line);
			break ;
		}
		if (!r->quoted)
		{
			exp = expand_variables(line, last_exit);
			write(pipe_fd[1], exp, ft_strlen(exp));
			free(exp);
		}
		else
			write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	fd = dup(pipe_fd[0]);
	close(pipe_fd[0]);
	return (fd);
}

/* Setup redirections */
static int	setup_redirections(t_redir *r, int *last_exit)
{
	int	fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
			fd = handle_heredoc(r, *last_exit);
		else if (r->type == REDIR_IN)
			fd = open(r->file, O_RDONLY);
		else if (r->type == REDIR_OUT)
			fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->file, 2);
			if (errno == EACCES)
				ft_putstr_fd(": Permission denied\n", 2);
			else
				ft_putstr_fd(": No such file or directory\n", 2);
			*last_exit = 1;
			return (1);
		}
		if (r->type == REDIR_IN || r->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
	return (0);
}

/* Child execution */
static void	exec_child(struct s_token *token, int *last_exit, char **env)
{
	char	*path;

	if (token->built_in)
		exit(execute_builtin(token, last_exit, &env));
	if (!token->argv[0] || !token->argv[0][0])
		exit(0);
	path = find_executable(token->argv[0], env);
	if (!path)
	{
		write(2, token->argv[0], ft_strlen(token->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	execve(path, token->argv, env);
	if (errno == EISDIR)
	{
		write(2, token->argv[0], ft_strlen(token->argv[0]));
		write(2, ": Is a directory\n", 17);
		free(path);
		exit(126);
	}
	if (errno == EACCES)
	{
		write(2, token->argv[0], ft_strlen(token->argv[0]));
		write(2, ": Permission denied\n", 20);
		free(path);
		exit(126);
	}
	write(2, token->argv[0], ft_strlen(token->argv[0]));
	write(2, ": ", 2);
	perror("");
	free(path);
	exit(126);
}

/* Fork and execute process */
static int	fork_and_execute(t_redir *r, struct s_token *token, int *last_exit,
							int in_fd, int out_fd, char **env, pid_t *pid)
{
	*pid = fork();
	if (*pid == 0)
	{
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		if (r && setup_redirections(r, last_exit))
			exit(*last_exit);
		exec_child(token, last_exit, env);
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0);
}

int	executor(struct s_token **tokens, int *last_exit, char ***env)
{
	int		in_fd;
	int		pipe_fd[2];
	int		out_fd;
	pid_t	*pids;
	int		cmd_count;
	int		i;

	out_fd = STDOUT_FILENO;
	in_fd = STDIN_FILENO;
	cmd_count = 0;
	while (tokens[cmd_count])
		cmd_count++;
	pids = malloc(cmd_count * sizeof(pid_t));
	if (!pids)
		return (*last_exit = 1);
	i = 0;
	while (i < cmd_count)
	{
		pipe_fd[0] = -1;
		if (tokens[i + 1])
		{
			if (pipe(pipe_fd) != 0)
			{
				free(pids);
				return (*last_exit = 1);
			}
			out_fd = pipe_fd[1];
		}
		else
			out_fd = STDOUT_FILENO;
		if (tokens[i]->built_in && in_fd == STDIN_FILENO
			&& out_fd == STDOUT_FILENO && !tokens[i]->redirs)
		{
			execute_builtin(tokens[i], last_exit, env);
			pids[i] = 0;
		}
		else
		{
			fork_and_execute(tokens[i]->redirs, tokens[i], last_exit,
				in_fd, out_fd, *env, &pids[i]);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (tokens[i + 1])
			in_fd = pipe_fd[0];
		else
			in_fd = STDIN_FILENO;
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		i++;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], last_exit, 0);
			*last_exit = WEXITSTATUS(*last_exit);
		}
		i++;
	}
	free(pids);
	return (*last_exit);
}
