#include "../minishell.h"

/**
 * @brief Change directory builtin command
 * @param argv Command arguments (argv[0] is "cd", argv[1] is target path or NULL)
 * @param env Environment variables
 * @return 0 on success, 1 on error
 */
int	builtin_cd(char **argv, char ***env)
{
	char	*path;
	char	*oldpwd;
	char	current_dir[4096];
	char	*pwd_var;
	int		ret;

	/* Get current directory before changing */
	if (getcwd(current_dir, 4096) == NULL)
		ft_strlcpy(current_dir, ".", 2);
	
	/* Determine target path */
	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (1);
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = argv[1];
	
	/* Save old PWD for OLDPWD environment variable */
	oldpwd = ft_strjoin("OLDPWD=", current_dir);
	if (!oldpwd)
		return (1);
	
	/* Change directory */
	ret = chdir(path);
	if (ret != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (1);
	}
	
	/* Update PWD environment variable */
	if (getcwd(current_dir, 4096) == NULL)
		ft_strlcpy(current_dir, ".", 2);
	
	pwd_var = ft_strjoin("PWD=", current_dir);
	if (!pwd_var)
	{
		free(oldpwd);
		return (1);
	}
	
	/* Update environment variables */
	char *new_vars[3] = {pwd_var, oldpwd, NULL};
	for (int i = 0; new_vars[i]; i++)
	{
		char **env_args = malloc(3 * sizeof(char *));
		if (!env_args)
		{
			free(pwd_var);
			free(oldpwd);
			return (1);
		}
		env_args[0] = ft_strdup("export");
		env_args[1] = new_vars[i];
		env_args[2] = NULL;
		builtin_export(env_args, env);
		free(env_args[0]);
		free(env_args);
	}
	
	free(pwd_var);
	free(oldpwd);
	return (0);
}
