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
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	execute_builtin(struct s_token *token, int *last_exit, char ***env)
{
	if (!token || !token->argv || !token->argv[0])
		return (*last_exit = 1);
	if (ft_strcmp(token->argv[0], "echo") == 0)
		return (*last_exit = builtin_echo(token->argv, token->quoted,
				*last_exit));
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

static void	command_not_found_exit(char *argv, int *last_exit)
{
	char	*la;

	if (ft_strcmp(argv, "$?") == 0)
	{
		la = ft_itoa(*last_exit);
		write(2, la, ft_strlen(la));
		free(la);
	}
	else
		write(2, argv, ft_strlen(argv));
	write(2, ": command not found\n", 20);
	exit(127);
}

static void	is_dir_exit(char *argv, char *path)
{
	write(2, argv, ft_strlen(argv));
	write(2, ": Is a directory\n", 17);
	if (path)
		free(path);
	exit(126);
}

static void	permission_denied_exit(char *argv, char *path)
{
	write(2, argv, ft_strlen(argv));
	write(2, ": Permission denied\n", 20);
	if (path)
		free(path);
	exit(126);
}

char	*find_executable(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*res;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strcmp(cmd, "$PWD") == 0)
	{
		path = getenv("PWD");
		is_dir_exit(cmd, NULL);
	}
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		return (ft_strdup(cmd));
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	res = search_in_dirs(cmd, dirs);
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	return (free(dirs), res);
}

static int	handle_heredoc(t_redir *r, int last_exit)
{
	int		pipe_fd[2];
	char	*line;
	char	*exp;
	int		fd;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, r->file) == 0)
			break ;
		if (!r->quoted)
		{
			exp = expand_variables(line, last_exit);
			ft_putendl_fd(exp, pipe_fd[1]);
			free(exp);
		}
		else
			ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	return (close(pipe_fd[1]), fd = dup(pipe_fd[0]), close(pipe_fd[0]), fd);
}

static void	fd_error(t_redir *r)
{
	ft_putstr_fd(ft_strjoin("minishell: ", r->file), 2);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}

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
			return (fd_error(r), *last_exit = 1, 1);
		if (r->type == REDIR_IN || r->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
	return (0);
}

static void	exec_child(struct s_token *token, int *last_exit, char ***env)
{
	char	*path;

	if (token->built_in)
		exit(execute_builtin(token, last_exit, env));
	if (!token->argv[0] || !token->argv[0][0])
		exit(0);
	path = find_executable(token->argv[0], *env);
	if (!path)
		command_not_found_exit(token->argv[0], last_exit);
	execve(path, token->argv, *env);
	if (errno == EISDIR)
		is_dir_exit(token->argv[0], path);
	if (errno == EACCES)
		permission_denied_exit(token->argv[0], path);
	write(2, token->argv[0], ft_strlen(token->argv[0]));
	write(2, ": ", 2);
	perror("");
	free(path);
	exit(126);
}

static int	fork_and_execute(struct s_token *token, int *last_exit,
		struct s_piper *piper, char ***env)
{
	piper->pids[piper->i] = fork();
	if (piper->pids[piper->i] == 0)
	{
		if (piper->in_fd != STDIN_FILENO)
		{
			dup2(piper->in_fd, STDIN_FILENO);
			close(piper->in_fd);
		}
		if (piper->out_fd != STDOUT_FILENO)
		{
			dup2(piper->out_fd, STDOUT_FILENO);
			close(piper->out_fd);
		}
		if (token->redirs && setup_redirections(token->redirs, last_exit))
			exit(*last_exit);
		exec_child(token, last_exit, env);
	}
	if (piper->in_fd != STDIN_FILENO)
		close(piper->in_fd);
	if (piper->out_fd != STDOUT_FILENO)
		close(piper->out_fd);
	return (0);
}

static int	prepare_fds(struct s_piper *piper, struct s_token **tokens)
{
	piper->pipe_fd[0] = -1;
	if (tokens[piper->i + 1])
	{
		if (pipe(piper->pipe_fd) != 0)
			return (1);
		piper->out_fd = piper->pipe_fd[1];
	}
	else
		piper->out_fd = STDOUT_FILENO;
	return (0);
}

static int	execute_cmd(struct s_token *tok, int *last_exit,
		struct s_piper *piper, char ***env)
{
	if (tok->built_in && piper->in_fd == STDIN_FILENO
		&& piper->out_fd == STDOUT_FILENO && !tok->redirs)
	{
		execute_builtin(tok, last_exit, env);
		piper->pids[piper->i] = 0;
	}
	else
	{
		if (fork_and_execute(tok, last_exit, piper, env) < 0)
			return (1);
	}
	return (0);
}

static void	update_fds(struct s_piper *piper, struct s_token **tokens)
{
	if (piper->in_fd != STDIN_FILENO)
		close(piper->in_fd);
	if (tokens[piper->i + 1])
		piper->in_fd = piper->pipe_fd[0];
	else
		piper->in_fd = STDIN_FILENO;
	if (piper->out_fd != STDOUT_FILENO)
		close(piper->out_fd);
}

int	execute_loop(struct s_piper *piper, struct s_token **tokens, int *last_exit,
		char ***env)
{
	piper->i = -1;
	while (++piper->i < piper->cmd_count)
	{
		if (prepare_fds(piper, tokens))
		{
			*last_exit = 1;
			return (1);
		}
		if (execute_cmd(tokens[piper->i], last_exit, piper, env))
		{
			*last_exit = 1;
			return (1);
		}
		update_fds(piper, tokens);
	}
	return (0);
}

int	executor(struct s_token **tokens, int *last_exit, char ***env)
{
	t_piper	piper;
	int		status;

	piper.out_fd = STDOUT_FILENO;
	piper.in_fd = STDIN_FILENO;
	piper.cmd_count = 0;
	while (tokens[piper.cmd_count])
		piper.cmd_count++;
	piper.pids = malloc(piper.cmd_count * sizeof(pid_t));
	if (!piper.pids)
		return (*last_exit = 1, 1);
	if (execute_loop(&piper, tokens, last_exit, env))
		return (free(piper.pids), *last_exit);
	if (piper.in_fd != STDIN_FILENO)
		close(piper.in_fd);
	piper.i = -1;
	while (++piper.i < piper.cmd_count)
	{
		if (piper.pids[piper.i] > 0)
		{
			waitpid(piper.pids[piper.i], &status, 0);
			*last_exit = WEXITSTATUS(status);
		}
	}
	return (free(piper.pids), *last_exit);
}
