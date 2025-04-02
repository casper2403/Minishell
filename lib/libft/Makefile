NAME = libft.a
CC = cc
CFLAGS = -Wall -Werror -Wextra -I. -g
CFILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_realloc.c
CFILES += ft_printf/ft_printf.c ft_printf/print_c.c ft_printf/print_d.c ft_printf/print_i.c ft_printf/print_p.c ft_printf/print_percent.c ft_printf/print_s.c ft_printf/print_u.c ft_printf/print_upperx.c ft_printf/print_x.c
CFILES += get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

OFILES = $(CFILES:%.c=obj/%.o)

all : $(NAME)

$(NAME) : $(OFILES)
	ar rcs $(NAME) $(OFILES)

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES)
	rm -rf obj

fclean : clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
