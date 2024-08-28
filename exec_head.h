/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_head.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:28:59 by eismail           #+#    #+#             */
/*   Updated: 2024/08/28 21:28:42 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <curses.h>
# include <term.h>
# include <errno.h>
# include "libft/libft.h"
# include "minishell.h"

// typedef struct s_env
// {
//     char *var;
//     char *value;
//     struct s_env *next;
    
// }   t_env;

t_env *creat_var(char *var);
void set_env(t_env **envi, char **env);
void add_env(t_env **head, t_env *env_new);
void free_env(t_env **env);
char **env_to_str(t_env *env);