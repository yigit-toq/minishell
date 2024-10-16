# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/26 17:02:40 by ytop              #+#    #+#              #
#    Updated: 2024/10/11 17:03:17 by abakirca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR		=	./sources/
OBJS_DIR		=	./objects/

COMPILER_DIR	=	$(SRCS_DIR)0-compiler/

EXECUTOR_DIR	=	$(SRCS_DIR)1-executor/

HEREDOC_DIR		=	$(SRCS_DIR)2-heredoc/

RDIRECT_DIR		=	$(SRCS_DIR)3-rdirect/

BUILTIN_DIR		=	$(SRCS_DIR)4-builtin/

DOLLAR_DIR		=	$(SRCS_DIR)5-dollar/

UTIL_DIR		=	$(SRCS_DIR)6-utils/

PIPE_DIR		=	$(SRCS_DIR)7-pipe/

SRCS			=	$(SRCS_DIR)minishell.c		$(COMPILER_DIR)lexer.c				$(COMPILER_DIR)parser.c				$(COMPILER_DIR)replace_arg.c	\
					$(EXECUTOR_DIR)executor.c	$(EXECUTOR_DIR)executor_utils_1.c	$(EXECUTOR_DIR)executor_utils_2.c									\
					$(HEREDOC_DIR)heredoc.c 	$(HEREDOC_DIR)heredoc_utils.c 		$(RDIRECT_DIR)redirect.c											\
					$(BUILTIN_DIR)cd.c			$(BUILTIN_DIR)cd_utils.c			$(BUILTIN_DIR)echo.c				$(BUILTIN_DIR)env.c 			\
					$(BUILTIN_DIR)exit.c 		$(BUILTIN_DIR)export.c				$(BUILTIN_DIR)pwd.c					$(BUILTIN_DIR)unset.c			\
					$(UTIL_DIR)check_line.c		$(UTIL_DIR)h_syntax.c				$(UTIL_DIR)quotes.c					$(UTIL_DIR)signal.c				\
					$(UTIL_DIR)split.c			$(UTIL_DIR)utils.c					$(UTIL_DIR)utils2.c													\
					$(PIPE_DIR)pipe.c			$(PIPE_DIR)pipe_utils.c																					\
					$(DOLLAR_DIR)dollar.c 		$(DOLLAR_DIR)dollar_utils.c																				\

OBJS			=	$(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

CC				=	@cc
CFLAGS			=	-Wall -Wextra -Werror $(INCLUDES)

INCLUDES		=	-I ./includes

RM				=	@rm -rf

NAME			=	minishell

LIBFT_MAKE		=	@make -C ./libraries/libft
LIBFT			=	./libraries/libft/libft.a

RDFLAGS			=	-lreadline

COLOR_YELLOW	=	\033[0;33m
COLOR_GREEN		=	\033[0;32m
COLOR_RED		=	\033[0;31m
COLOR_END		=	\033[0m

all				:	$(NAME)

$(NAME)			:	$(LIBFT) $(OBJS)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(RDFLAGS)
					@printf "MINISHELL		$(COLOR_GREEN)\e[1m[OK]\e[0m\n$(COLOR_END)"

$(OBJS_DIR)%.o	:	$(SRCS_DIR)%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) -c $< -o $@

clean			:
					$(RM) $(OBJS_DIR)
					$(LIBFT_MAKE) clean
					@printf "OBJECT FILES		$(COLOR_RED)\e[1m[KO]\e[0m\n$(COLOR_END)"

fclean			:	clean
					$(RM) $(NAME)
					$(LIBFT_MAKE) fclean
					@printf "EXECUTABLE FILES	$(COLOR_RED)\e[1m[KO]\e[0m\n$(COLOR_END)"

re				:	fclean all

$(LIBFT)		:
					$(LIBFT_MAKE) all

.PHONY			:	all clean fclean re
