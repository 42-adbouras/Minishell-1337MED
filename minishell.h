/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/30 17:58:40 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <curses.h>
# include <term.h>
# include "libft/libft.h"

typedef struct s_cmd
{
    char			*path;
    char			*rl;
    char			*args;
    char			*cmd;
    char			*option;
    char			**split;
    char			**red_in;
    char			**red_out;
    struct s_cmd	*pipe;
}					t_cmd;

/****************************	adbouras	****************************/
/***    utils.c   ****************************/
void	node_add_back(t_cmd **lst, t_cmd *new);
t_cmd	*new_node(char *content);
void	clear_nodes(t_cmd **list);

/***    clean.c   ****************************/
void	free_char_arr(char **arr);
void	ft_exit(t_cmd *data);

/***	eismail		****************************/
void	ft_pwd();

#endif