/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/25 10:09:10 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>

typedef struct s_cmd
{
    char            *path;
    char            *option;
    char            *args;
    char            *red_in;
    char            *red_out;
    struct s_cmd    *pipe;
}                   t_cmd;

/***	adbouras	****************************/
void	_func();
void	_leaks(void);

/***	eismail		****************************/
void	ft_pwd();

#endif
