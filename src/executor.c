#include "minishell.h"

/* Extract variable name from string */
static char	*get_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

/* Append src to *dest, free old */
static void	append_str(char **dest, const char *src)
{
	char	*old;

	old = *dest;
	*dest = ft_strjoin(old, src);
	free(old);
}

/* Append single char to *dest */
static void	append_char(char **dest, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	append_str(dest, tmp);
}

/* Extract $VAR or $? value, set len */
static char	*extract_value(const char *s, int *len, int last_exit)
{
	char	*var;
	char	*val;

	if (s[1] == '?')
	{
		*len = 2;
		return (ft_itoa(last_exit));
	}
	var = get_var_name((char *)s + 1);
	val = getenv(var);
	*len = ft_strlen(var) + 1;
	free(var);
	if (val)
		return (ft_strdup(val));
	else
		return (ft_strdup(""));
}

char	*expand_variables(char *str, int last_exit)
{
	char	*res;
	int		i;
	int		len;
	char	*v;

	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			v = extract_value(str + i, &len, last_exit);
			append_str(&res, v);
			free(v);
			i += len;
		}
		else
		{
			append_char(&res, str[i]);
			i++;
		}
	}
	return (res);
}

int	execute_builtin(struct s_token *token, int *last_exit, char ***env)
{
	if (!token || !token->argv || !token->argv[0])
		return (*last_exit = 1);
	if (ft_strcmp(token->argv[0], "echo") == 0)
		return (*last_exit = builtin_echo(token->argv));
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
		builtin_exit(token->argv);
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
static void	setup_redirections(t_redir *r, int last_exit)
{
	int	fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
			fd = handle_heredoc(r, last_exit);
		else if (r->type == REDIR_IN)
			fd = open(r->file, O_RDONLY);
		else if (r->type == REDIR_OUT)
			fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror(r->file);
			exit(1);
		}
		if (r->type == REDIR_IN || r->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
}

/* Child execution */
static void	exec_child(struct s_token *token, int *last_exit, char **env)
{
	char	*path;

	if (token->built_in)
		exit(execute_builtin(token, last_exit, &env));
	path = find_executable(token->argv[0], env);
	if (!path)
	{
		write(2, token->argv[0], ft_strlen(token->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	execve(path, token->argv, env);
	free(path);
	perror(token->argv[0]);
	exit(126);
}

/* Fork and execute process */
static int	fork_and_execute(t_redir *r, struct s_token *token, int *last_exit,
		int in_fd, int out_fd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
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
		if (r)
			setup_redirections(r, *last_exit);
		exec_child(token, last_exit, env);
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	waitpid(pid, last_exit, 0);
	*last_exit = WEXITSTATUS(*last_exit);
	return (*last_exit);
}

int	execute_command(struct s_token *token, int *last_exit, int in_fd,
		int out_fd, char **env)
{
	if (token->built_in && in_fd == STDIN_FILENO && out_fd == STDOUT_FILENO
		&& !token->redirs)
		return (execute_builtin(token, last_exit, &env));
	return (fork_and_execute(token->redirs, token, last_exit, in_fd, out_fd,
			env));
}

int	executor(struct s_token **tokens, int *last_exit, char ***env)
{
	int	in_fd;
	int	i;
	int	pipe_fd[2];
	int	out_fd;

	in_fd = STDIN_FILENO;
	i = 0;
	while (tokens[i])
	{
		out_fd = STDOUT_FILENO;
		if (tokens[i + 1] && pipe(pipe_fd) == 0)
			out_fd = pipe_fd[1];
		execute_command(tokens[i], last_exit, in_fd, out_fd, *env);
		if (tokens[i + 1])
			in_fd = pipe_fd[0];
		i++;
	}
	return (*last_exit);
}
