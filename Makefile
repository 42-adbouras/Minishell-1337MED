# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 08:58:18 by adbouras          #+#    #+#              #
#    Updated: 2024/08/08 09:13:14 by adbouras         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GRN	= \033[1;92m
YLW	= \033[1;93m
RED	= \033[1;91m
RST	= \033[0;39m

CC = cc

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

LDFLAGS = -lreadline

SRC =  main.c utils.c print.c lexer.c list_utils.c syntax.c parse_utils.c

LIBFT = libft/libft.a

OBJ_DIR = objects/

HDR = minishell.h

NCLD = -I/Users/adbouras/.brew/opt/readline/include

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
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -L/Users/adbouras/.brew/opt/readline/lib $(LDFLAGS) -o $(NAME)
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
