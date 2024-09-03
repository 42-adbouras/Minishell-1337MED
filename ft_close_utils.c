/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:23:42 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 12:30:03 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_clear(int cmd_num, t_fd *fd, int *pids)
{
	int i;
	int exit_status;
	int status;

	i = 0;
	exit_status = g_status;
	ft_close(cmd_num, fd->pipes, NULL);
	while (i < cmd_num)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				exit_status = 130;
			else if (WTERMSIG(status) == SIGQUIT)
				exit_status = 131;
		}
		i++;
	}
	free(pids);
	free(fd->fds);
	free_int(fd->pipes, cmd_num);
	free(fd);
	g_status = exit_status;
}

void close_fds(t_fd **fd)
{
	close ((*fd)->fds[0]);
	close ((*fd)->fds[1]);
	free((*fd)->fds);
	(*fd)->fds = NULL;
}

void free_fds(int *pids, t_fd **fd, int cmd_num)
{
	free(pids);
	free_int((*fd)->pipes, cmd_num);
	free((*fd)->fds);
	free(*fd);
}

void	free_int(int **p, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(p[i]);
		i++;
	}
	free(p);
}

