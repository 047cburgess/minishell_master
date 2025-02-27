# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caburges <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/21 11:33:25 by caburges          #+#    #+#              #
#    Updated: 2024/11/25 12:31:55 by caburges         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftdprintf.a

LIBFT = ../libft/libft.a

LIBFT_DIR = ../libft

SRCS = ft_printf.c \
	ft_printchar.c \
	ft_printstr.c \
	ft_printnbr.c \
	ft_printunbr.c \
	ft_printaddress.c \
	ft_printhex.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	cp $(LIBFT) $(NAME)
	ar rcs $(NAME) $(OBJS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)	
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: clean fclean re all
