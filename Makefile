# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/20 13:00:20 by ltouret           #+#    #+#              #
#    Updated: 2021/05/09 22:39:49 by ltouret          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .c .o

NAME = minishell

SRCS = srcs/main.c srcs/main_utils1.c srcs/main_utils2.c srcs/errors.c\
		srcs/parse_line.c srcs/parse_cmds.c srcs/parse_redirs.c\
		srcs/parse_redirs_utils.c srcs/post_process.c\
		srcs/post_process_utils1.c srcs/post_process_utils2.c\
		srcs/environ.c srcs/getters.c srcs/setters.c srcs/environ_utils1.c\
		srcs/environ_utils2.c srcs/exec_line.c srcs/exec_pipe_cmd.c\
		srcs/builtins.c srcs/cd.c srcs/echo.c srcs/env.c srcs/exit.c\
		srcs/export.c srcs/pwd.c srcs/unset.c srcs/whereis.c\
		srcs/path_utils.c srcs/exists.c srcs/join.c srcs/history.c

OBJS = ${SRCS:.c=.o}

CC		= gcc
RM		= rm -f

CFLAGS = -Wall -Wextra -Werror

FSAN = -fsanitize=address

LIBFT_FLAGS	= -Llibft -lft

INCLUDES	= -I libft

CURSES		= -lncurses

.c.o:
		${CC} ${INCLUDES} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): ${OBJS}
		make -C libft 
		${CC} ${OBJS} ${LIBFT_FLAGS} -o ${NAME} ${CURSES} 

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
		@reset

test2:	all
		@echo -n heeeeeeeeeeeeeeeey ; ./minishell
		@reset

.PHONY: all clean fclean re test test2
