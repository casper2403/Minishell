# Project Name

Some intro...

## TODO
<!-- TODO-START -->
- `./src/free_functions.c:5`
```c
free insides too!
```


- `./src/process.c:183`
```c
check all redirectors and keep rest as args
redirectors can be stuck to their args
make sure that everything is malloced
```


- `./src/process.c:190`
```c
incorrect! should keep track of quotes!
check if full fixed
```


- `./src/process.c:286`
```c
for loop norm
```


- `./src/process.c:376`
```c
PARSER
check syntax
```


- `./src/process.c:380`
```c
EXECUTOR
pid_t pid;

printf("%s\n", getenv("PATH"));
pid = fork();
char *command[] = {"/usr/bin/ls", "-l" , "src", NULL};
if (pid == 0)
execve(command[0], command, NULL);
wait(NULL);
```


- `./src/builtin/builtin_exit.c:3`
```c
remove prinf
```


- `./src/builtin/builtin_pwd.c:4`
```c
remove printf
```


<!-- TODO-END -->
