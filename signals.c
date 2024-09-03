/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:35:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 11:47:43 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	herdoc_signal(int sig)
{
	if (sig == SIGINT)
		exit(1);
}

void	signals_init(void)
{
	t_sa	sa;

	ft_bzero(&sa, sizeof(t_sa));
	sa.sa_flags = SA_RESTART;
	sa.sa_sigaction = &sig_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
