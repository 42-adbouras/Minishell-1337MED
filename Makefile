# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eismail <eismail@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 08:58:18 by adbouras          #+#    #+#              #
#    Updated: 2024/09/02 16:46:51 by eismail          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GRN	= \033[1;92m
YLW	= \033[1;93m
RED	= \033[1;91m
RST	= \033[0;39m

CC = cc

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

LDFLAGS = -lreadline -L/Users/adhambouras/.brew/opt/readline/lib

SRC =  main.c utils.c print.c lexer.c parse_list_utils.c syntax.c parse_utils.c clean.c error.c env.c \
		lexer_utils.c redir_utils.c arg_utils.c lexer_list_utils.c builtins.c ft_exic.c signals.c ft_readline.c \
		getters.c process_redir_utils.c expand_utils.c expand_utils_2.c \

LIBFT = libft/libft.a

OBJ_DIR = objects/

HDR = minishell.h

NCLD = -I/Users/adhambouras/.brew/opt/readline/include

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

NAME = minishell

all: $(NAME)

$(OBJ_DIR)%.o:%.c $(HDR)
	@echo "$(YLW)[Creating Object $@]$(RST)"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(NCLD) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(YLW)[Compiling libft]$(RST)"
	@make -C libft
	@echo "$(GRN)[libft Compiled]$(RST)" && sleep 1
	@echo "$(YLW)[Compiling ...]$(RST)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GRN)[Compiled]$(RST)"

clean:
	@make clean -C libft
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[Removed Objects]$(RST)"

fclean: clean
	@make fclean -C libft
	@rm -rf $(NAME)
	@echo "$(RED)[Removed $(NAME)]$(RST)"

re: fclean all
