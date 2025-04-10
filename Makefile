NAME_EXEC	= minishell
LIBFT		= lib/libft/libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -g -lreadline
OFLAGS		= -Wall -Wextra -g
OBJ			= obj

# Source files
EXEC_SRCS	= src/main.c src/process.c src/free_functions.c

# Object files in the obj folder
EXEC_OBJS	= $(EXEC_SRCS:src/%.c=obj/%.o)

all:		$(LIBFT) $(NAME_EXEC)

$(NAME_EXEC):	$(EXEC_OBJS)
		$(CC) $(EXEC_OBJS) $(CFLAGS) $(LIBFT) -o $(NAME_EXEC)

obj/%.o: src/%.c | $(OBJ)
		$(CC) $(OFLAGS) -c $< -o $@

$(OBJ):
		mkdir -p $(OBJ)

$(LIBFT):
		make -C lib/libft

clean:
		rm -rf $(OBJ)
		make -C lib/libft clean

fclean:		clean
		rm -f $(NAME_EXEC)

re:		fclean all

.PHONY:		all clean fclean re
