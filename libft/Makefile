# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/14 21:48:50 by adbouras          #+#    #+#              #
#    Updated: 2024/08/06 17:13:08 by adbouras         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GRN	= \033[1;92m
YLW	= \033[1;93m
RED	= \033[1;91m
RST	= \033[0;39m

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

HEADER = libft.h

SOURCE =	ft_atoi.c \
		ft_strchr.c \
		ft_bzero.c \
		ft_strdup.c \
		ft_calloc.c \
		ft_strjoin.c \
		ft_isalnum.c \
		ft_strlcat.c \
		ft_isalpha.c \
		ft_strlcpy.c \
		ft_isascii.c \
		ft_strlen.c \
		ft_isdigit.c \
		ft_strncmp.c \
		ft_isprint.c \
		ft_strnstr.c \
		ft_memchr.c \
		ft_strrchr.c \
		ft_memcmp.c \
		ft_substr.c \
		ft_memcpy.c \
		ft_tolower.c \
		ft_memmove.c \
		ft_toupper.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c	\
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_strtrim.c \
		ft_itoa.c \
		ft_split.c \
		ft_strmapi.c \
		ft_striteri.c \


BNS =	ft_lstnew_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstiter_bonus.c \
		ft_lstmap_bonus.c \

OBJ_DIR = objects/

BNS_OBJC = $(addprefix $(OBJ_DIR), $(BNS:.c=.o))

OBJECT = $(addprefix $(OBJ_DIR), $(SOURCE:.c=.o))

$(OBJ_DIR)%.o:%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
 
$(NAME): $(OBJECT) $(HEADER) 
	@ar -cr $(NAME) $(OBJECT)
	
all: $(NAME)

bonus: $(BNS_OBJC) $(HEADER)
	@ar -cr $(NAME) $(BNS_OBJC)

clean:
	@rm -rf $(OBJ_DIR)

fclean:	clean
	@rm -f $(NAME)
	@echo "$(RED)[Removed $(NAME)]$(RST)"

re:	fclean all