# Project Name

Some intro...

## TODO
<!-- TODO-START -->
- `./src/process.c:10`
```c
check all redirectors and keep rest as args
make sure that everything is malloced
```


- `./src/process.c:98`
```c
PARSER
check syntax
```


- `./src/process.c:102`
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


- `./src/free_functions.c:5`
```c
free insides too!
```


<!-- TODO-END -->
