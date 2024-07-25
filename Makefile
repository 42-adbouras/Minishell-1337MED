# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eismail <eismail@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 08:58:18 by adbouras          #+#    #+#              #
#    Updated: 2024/07/25 09:51:19 by eismail          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GRN	= \033[1;92m
YLW	= \033[1;93m
RED	= \033[1;91m
RST	= \033[0;39m

CC = cc

CFLAGS = -Wall -Wextra -Werror 

SRC =  main.c minishell.c exic.c utils.c

LIBFT = libft/libft.a

OBJ_DIR = objects/

HDR = minishell.h

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

NAME = minishell

all: $(NAME)

$(OBJ_DIR)%.o:%.c $(HDR)
	@echo "$(YLW)[Creating Object $@]$(RST)"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(YLW)[Compiling ...]$(RST)"
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "$(GRN)[Compiled]$(RST)"

clean:
	@make clean -C libft
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[Removed Objects]$(RST)"

fclean: clean
	@make fclean -C libft
	@rm -rf $(NAME)
	@echo "$(RED)[Removed EXE]$(RST)"

re: fclean all