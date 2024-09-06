/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:12:05 by eismail           #+#    #+#             */
/*   Updated: 2024/09/06 11:38:47 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_close_pip(int **pip, int i)
{
	int l;

	l = -1;
	while (++l < i)
	{
		close(pip[l][0]);
		close(pip[l][1]);	
	}
}

int	**ft_pip(int cmd_num)
{
	int	i;
	int	**fd;

	i = 0;
	fd = malloc(sizeof(int *) * cmd_num);
	if (!fd)
		return (NULL);
	while (i < cmd_num)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i++])
		{
			free_int(fd, i);
			return (NULL);
		}
	}
	i = 0;
	while ((i < cmd_num - 1))
	{
		if (pipe(fd[i]) == -1)
			return (ft_close_pip(fd, i), free_int(fd, cmd_num), NULL);
		i++;
	}
	return (fd);
}

void	ft_close(int cmd_num, int **pipes, int *fds)
{
	int	i;

	i = 0;
	while (i < cmd_num - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	if (fds && fds[0] != -1)
		close(fds[0]);
	if (fds && fds[1] != -1)
		close(fds[1]);
}

void	ft_stdin(int **pipes, int pid, int *fds)
{
	if (fds && fds[0] != -1)
	{
		dup2(fds[0], STDIN_FILENO);
	}
	else if (pid != 0)
	{
		dup2(pipes[pid - 1][0], STDIN_FILENO);
	}
}

void	ft_stdout(int cmd_num, int **pipes, int pid, int *fds)
{
	if (fds && fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
	}
	else if (pid != cmd_num - 1)
	{
		dup2(pipes[pid][1], STDOUT_FILENO);
	}
}

bool	fd_hindler(int cmd_num, int **fd, int *fds, int pos)
{
	ft_stdin(fd, pos, fds);
	ft_stdout(cmd_num, fd, pos, fds);
	ft_close(cmd_num, fd, fds);
	return (true);
}
