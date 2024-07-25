/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/24 11:16:42 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_data
{
	char	*rl;
    char	*path;
    char	*flag;
    int		cmd;
	t_data	*next;
}			t_data;

/***	adbouras	****************************/
void	_func();
void	_leaks(void);

/***	eismail		****************************/
void	ft_pwd();

#endif
