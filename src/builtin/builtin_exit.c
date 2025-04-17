#include "../minishell.h"

//TODO remove prinf
int builtin_exit(char **argv) {
    printf("exit\n");
    exit(argv[1] ? ft_atoi(argv[1]) : 0);
}
