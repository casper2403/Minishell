#include "../minishell.h"

//TODO remove printf
int builtin_pwd(char **argv) {
    char *pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
    free(pwd);
    return (0);
}
