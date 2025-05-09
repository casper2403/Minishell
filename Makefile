# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 10:47:04 by cstevens          #+#    #+#              #
#    Updated: 2025/05/08 10:47:53 by cstevens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_EXEC	= minishell
LIBFT		= lib/libft/libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -g -Isrc -Ilib/libft
OFLAGS		= -Wall -Wextra -g -Isrc -Ilib/libft
LDFLAGS		= -lreadline
OBJ			= obj

# Source files
EXEC_SRCS	= src/args_counting.c \
			  src/executor.c \
			  src/executor_utils.c \
			  src/free_functions.c \
			  src/main.c \
			  src/parser.c \
			  src/parser_utils.c \
			  src/process_redirection.c \
			  src/redirections.c \
			  src/split_arguments.c \
			  src/split_arguments_utils.c \
			  src/split_input.c \
			  src/split_input_utils.c \
			  src/signals.c
EXEC_SRCS	+= src/builtin/builtin_cd.c src/builtin/builtin_echo.c src/builtin/builtin_env.c
EXEC_SRCS	+= src/builtin/builtin_exit.c src/builtin/builtin_export_utils.c src/builtin/builtin_export.c src/builtin/builtin_pwd.c src/builtin/builtin_unset.c

# Object files in the obj folder (handle subdirectories)
EXEC_OBJS	= $(EXEC_SRCS:src/%.c=obj/%.o)

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
