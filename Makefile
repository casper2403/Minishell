NAME_EXEC	= minishell
LIBFT		= lib/libft/libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -g -Isrc -Ilib/libft
OFLAGS		= -Wall -Wextra -g -Isrc -Ilib/libft
LDFLAGS		= -lreadline
OBJ			= obj

# Source files
EXEC_SRCS	= src/main.c src/process.c src/free_functions.c src/executor.c
EXEC_SRCS	+= src/builtin/builtin_cd.c src/builtin/builtin_echo.c src/builtin/builtin_env.c
EXEC_SRCS	+= src/builtin/builtin_exit.c src/builtin/builtin_export.c src/builtin/builtin_pwd.c src/builtin/builtin_unset.c

# Object files in the obj folder (handle subdirectories)
EXEC_OBJS	= $(patsubst src/%.c,obj/%.o,$(EXEC_SRCS))

all:		$(LIBFT) $(NAME_EXEC)

$(NAME_EXEC):	$(EXEC_OBJS)
		$(CC) $(CFLAGS) $(EXEC_OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME_EXEC)

obj/%.o: src/%.c | $(OBJ)
		@mkdir -p $(dir $@)
		$(CC) $(OFLAGS) -c $< -o $@

$(OBJ):
		mkdir -p $(OBJ) $(OBJ)/builtin

$(LIBFT):
		make -C lib/libft

clean:
		rm -rf $(OBJ)
		make -C lib/libft clean

fclean:		clean
		rm -f $(NAME_EXEC)
		make -C lib/libft fclean

re:		fclean all

.PHONY:		all clean fclean re