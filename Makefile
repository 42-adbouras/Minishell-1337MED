# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 08:58:18 by adbouras          #+#    #+#              #
#    Updated: 2024/09/21 16:45:53 by adbouras         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GRN	= \033[1;92m
YLW	= \033[1;93m
RED	= \033[1;91m
CYN = \033[1;96m
RST	= \033[0;39m

CC = cc

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

LDFLAGS = -lreadline -L/Users/adbouras/.brew/opt/readline/lib

SRC =  main.c utils.c lexer.c parse_list_utils.c syntax.c parse_utils.c clean.c error.c \
		lexer_utils.c redir_utils.c arg_utils.c lexer_list_utils.c builtins.c ft_exic.c signals.c ft_readline.c \
		getters.c process_redir_utils.c expand_utils.c expand_utils_2.c ft_cd.c ft_echo.c ft_env.c ft_export.c ft_unset.c \
		ft_exit.c ft_pwd.c ft_heredoc.c ft_close_utils.c ft_env_utils.c ft_exic_utils.c ft_open.c ft_redirection.c word_count_utils.c \

LIBFT = libft/libft.a

OBJ_DIR = objects/

HDR = minishell.h

LIBFT_HDR = ./libft/libft.h

NCLD = -I/Users/adbouras/.brew/opt/readline/include

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

NAME = minishell

all: $(NAME)


$(OBJ_DIR)%.o:%.c $(HDR) $(LIBFT_HDR)
	@echo "$(YLW)[Creating Object $@]$(RST)"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(NCLD) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(YLW)[Compiling libft]$(RST)"
	@make -C libft
	@echo "$(GRN)[libft Compiled]$(RST)"
	@echo "$(YLW)[Compiling ...]$(RST)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GRN)[Compiled]$(RST)"
	@echo "$(CYN)▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄"
	@echo "█						█"
	@echo "█	Devoloped by:				█"
	@echo "█  adbouras ->	Parser				█"
	@echo "█  eismail  ->	Executer			█"
	@echo "█  						█"
	@echo "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄$(RST)"

clean:
	@make clean -C libft
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[Removed Objects]$(RST)"

fclean: clean
	@make fclean -C libft
	@rm -rf $(NAME)
	@echo "$(RED)[Removed $(NAME)]$(RST)"

re: fclean all
