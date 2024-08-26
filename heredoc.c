/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:05:33 by eismail           #+#    #+#             */
/*   Updated: 2024/08/26 12:13:45 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void read_heredoc(char *delimiter, int *pip, t_env *env)
{
	char *s;
	char *line;
	
	s = ft_strjoin(delimiter, "\n");
	line = ft_strdup("");
	while (ft_strncmp(line, s, ft_strlen(s) + 1) != 0)
	{
		ft_putstr_fd(line, pip[1]);
		free(line);
		line = readline("heredoc> ");
		if (!line)
        {
            free(s);
            exit(130);
        }
        if (line[0] == '$')
            line = ft_expand(env, &line[1]);
		line = ft_strjoin(line, "\n");
	}
	free(line);
	free(s);
	exit(0);
}

void herdoc_signal(int sig)
{
	(void)sig;
	exit(130);
}

void if_herdoc(char **delimiters, t_env *env, int *fd_heredoc)
{
	int i;
	int *pip;
	int pid;

	i = -1;
    *(fd_heredoc) = -1;
	if (!delimiters || !(*delimiters) )
		return ;
	pip = malloc(sizeof(int) * 2);
	while (delimiters[++i])
	{
		if (pipe(pip) == -1)
			return ;
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, herdoc_signal);
			read_heredoc(delimiters[i], pip, env);
		}
		waitpid(pid, &g_status, 0);
	}
	close(pip[1]);
	*(fd_heredoc) = pip[0];
	free(pip);
}
