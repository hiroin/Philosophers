# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/20 19:50:14 by hkamiya           #+#    #+#              #
#    Updated: 2020/11/05 07:48:41 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= convert_to_milliseconds.c \
		  diff_time.c \
		  ft_atoi.c \
		  ft_ltoa.c \
		  ft_putstr.c \
		  ft_strjoin.c \
		  ft_strlen.c \
		  init_t_philosopher.c \
		  main.c \
		  set_t_params.c \
		  validate_argv.c \
		  write_message.c
OBJS	= $(SRCS:.c=.o)
CC		= clang-9
RM		= rm -f
#CFLAGS	= -g -fsanitize=undefined,address -Wall -Wextra -Werror
CFLAGS	= -g -Wall -Wextra -Werror
LIB		= -pthread
NAME	= philo_one
all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIB) $(OBJS) -o $(NAME)
clean:
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
ver:
	$(CC) --version