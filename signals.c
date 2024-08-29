/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:35:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/29 16:38:29 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigquit_handler(int sig)
{
	if (sig == SIGQUIT)
		return;
}
	

void	sig_handler(int sig, siginfo_t *siginfo, void *ptr)
{
    (void) siginfo;
    (void) ptr;
	if (sig == SIGINT)
    {
        g_status = 1;
        write (1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

// void	catch_sigint(int signum)
// {
// 	(void)signum;
//     g_status = 1;
// 	// ft_putstr_fd("\n", 1);
//      write (1, "\n", 1);
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

// void	catch_sigquit(int signum)
// {
// 	(void)signum;
// 	kill(0, SIGCHLD);
// }

void    signals_init()
{
    t_sa    sa;

    ft_bzero(&sa, sizeof(t_sa));
    sa.sa_flags = 0;
    sa.sa_sigaction = &sig_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaction(SIGINT, &sa, NULL);
}
