/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:35:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/23 12:01:57 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigquit_handler(int sig)
{
	if (sig == SIGQUIT)
		return;
}
	

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
    {
        g_status = 1;
        write (1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void    signals_init()
{
    t_sa	sa_int;
    t_sa	sa_quit;

    rl_catch_signals = 0;
    sa_int.sa_flags = 0;
    sa_quit.sa_flags = 0;
	sa_int.sa_handler = &sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
    
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
