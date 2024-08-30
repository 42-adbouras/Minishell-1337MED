/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:35:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/30 10:46:15 by eismail          ###   ########.fr       */
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

// void handle_sigchld(int sig) {
// 	(void)sig;
//     // g_status = 130;
//     // int status;
//     // int status2;
//     // Wait for all terminated child processes
//     while (waitpid(-1, NULL , WNOHANG) > 0)
//     {
//         g_status = 130;
//     }
// }

void    signals_init()
{
    t_sa    sa;
    
    ft_bzero(&sa, sizeof(t_sa));
    sa.sa_flags = SA_RESTART;
    // rl_catch_signals = 0;
    sa.sa_sigaction = &sig_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
