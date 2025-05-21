# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cstevens <cstevens@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 10:47:04 by cstevens          #+#    #+#              #
#    Updated: 2025/05/22 22:27:40 by cstevens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_EXEC	= minishell
LIBFT		= lib/libft/libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g -o -Isrc -Ilib/libft
OFLAGS		= -Wall -Wextra -Werror -g -o -Isrc -Ilib/libft
LDFLAGS		= -lreadline
OBJ			= obj

# Source files
EXEC_SRCS	= src/args_counting.c \
			  src/executor.c \
			  src/executor_utils.c \
			  src/executor_utils2.c \
			  src/executor_utils3.c \
			  src/executor_utils4.c \
			  src/executor_utils5.c \
			  src/executor_utils6.c \
			  src/executor_utils7.c \
			  src/executor_utils8.c \
			  src/free_functions.c \
			  src/main.c \
			  src/parser.c \
			  src/parser_utils.c \
			  src/parser_utils2.c \
			  src/process_redirection.c \
			  src/redirections.c \
			  src/split_arguments.c \
			  src/split_arguments_utils.c \
			  src/split_input.c \
			  src/split_input_utils.c \
			  src/split_input_utils2.c \
			  src/signals.c \
			  src/copyenv_and_getvarname.c \
			  src/ulc.c \
			  src/extras.c
EXEC_SRCS	+= src/builtin/builtin_cd.c src/builtin/builtin_cd_utils.c src/builtin/builtin_echo.c src/builtin/builtin_env.c
EXEC_SRCS	+= src/builtin/builtin_exit.c src/builtin/builtin_export_utils.c src/builtin/builtin_export.c src/builtin/builtin_pwd.c src/builtin/builtin_unset.c src/builtin/builtin_export_utils2.c

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
