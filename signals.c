/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:35:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/07 19:03:29 by eismail          ###   ########.fr       */
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

void	child_signal(int sig)
{
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("quit\n", 2);
		exit(1);
	}
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
