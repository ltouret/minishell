# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/20 13:00:20 by ltouret           #+#    #+#              #
#    Updated: 2021/04/01 19:50:19 by ltouret          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .c .o

NAME = minishell

SRCS = srcs/main.c srcs/main_utils1.c srcs/errors.c srcs/parse_line.c\
		srcs/parse_cmds.c

OBJS = ${SRCS:.c=.o}

CC		= gcc
RM		= rm -f

CFLAGS = -Wall -Wextra -Werror

FSAN = -fsanitize=address

LIBFT_FLAGS	= -Llibft -lft

INCLUDES	= -I libft

.c.o:
		${CC} ${INCLUDES} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): ${OBJS}
		make -C libft 
		${CC} ${OBJS} ${LIBFT_FLAGS} -o ${NAME} 

all:	${NAME}

clean:
		make -C libft clean
		${RM} ${OBJS}

fclean:	clean
		rm -f libft/libft.a
		${RM} ${NAME}

re:		fclean all

test:	all
		@./minishell

.PHONY: all clean fclean re
