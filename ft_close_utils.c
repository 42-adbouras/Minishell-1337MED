/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:23:42 by eismail           #+#    #+#             */
/*   Updated: 2024/09/07 15:51:32 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear(int cmd_num, t_fd *fd, int *pids)
{
	int	i;
	int	exit_status;
	int	status;

	i = -1;
	exit_status = g_status;
	ft_close(cmd_num, fd->pipes, NULL);
	while (++i < cmd_num)
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
	}
	free_fds(pids, &fd, cmd_num);
	g_status = exit_status;
}

void	close_fds(t_fd **fd, int cmd_num)
{
	int	i;

	i = 0;
	while (fd && *fd && (*fd)->fds && (*fd)->fds[i] && i < cmd_num)
	{
		close((*fd)->fds[i][0]);
		close((*fd)->fds[i][1]);
		i++;
	}
	i = 0;
	while (fd && *fd && (*fd)->pipes && (*fd)->pipes[i] && i < cmd_num - 1)
	{
		close((*fd)->pipes[i][0]);
		close((*fd)->pipes[i][1]);
		i++;
	}
}

void	free_fds(int *pids, t_fd **fd, int cmd_num)
{
	int	i;

	i = 0;
	if (g_status == 300)
		g_status = 1;
	close_fds(fd, cmd_num);
	free(pids);
	free_int((*fd)->pipes, cmd_num);
	free_int((*fd)->fds, cmd_num);
	free(*fd);
}

void	free_int(int **p, int n)
{
	int	i;

	i = n - 1;
	while (i >= 0)
	{
		if (p && p[i])
			free(p[i]);
		i--;
	}
	if (p)
		free(p);
}

void	ft_close_pip(int **pip, int i)
{
	int	l;

	l = -1;
	while (++l < i)
	{
		close(pip[l][0]);
		close(pip[l][1]);
	}
}
