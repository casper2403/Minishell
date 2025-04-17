#include "../minishell.h"

int builtin_env(char **argv, char ***env) 
{
    for (int i = 0; (*env)[i]; i++)
        printf("%s\n", (*env)[i]);
    return (0);
}
