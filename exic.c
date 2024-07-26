/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exic.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:09:01 by eismail           #+#    #+#             */
/*   Updated: 2024/07/26 09:33:27 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>
#include <fcntl.h>

void ft_pwd()
{
	char s[100];
	getcwd(s,100);
	printf("%s\n",s);
}

int ft_count_cmd(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp->pipe)
	{
		i++;
		tmp = tmp->pipe;
	}
	return (i);
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

int	**ft_pip(int cmd_num)
{
	int	i;
	int	**fd;

	i = 0;
	fd = malloc(sizeof(int *) * cmd_num);
	if (!fd)
		ft_error(NULL, 1);
	while (i < cmd_num)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i++])
		{
			free_int(fd, i);
			ft_error(NULL, 1);
		}
	}
	i = 0;
	while ((i < cmd_num - 1))
	{
		if (pipe(fd[i]) == -1)
			return (free_int(fd, cmd_num), ft_error(NULL, 1), NULL);
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
	close(fds[0]);
	close(fds[1]);
}

void	ft_stdin(int **pipes, int pid, int *fds)
{
	if (pid == 0)
		dup2(fds[0], STDIN_FILENO);
	if (pid != 0)
		dup2(pipes[pid - 1][0], STDIN_FILENO);
}

void	ft_stdout(int cmd_num, int **pipes, int pid, int *fds)
{
	if (pid == cmd_num - 1)
		dup2(fds[1], STDOUT_FILENO);
	if (pid != cmd_num - 1)
		dup2(pipes[pid][1], STDOUT_FILENO);
}

bool fd_hindler(t_cmd *cmd, int cmd_num, int pos)
{
	int fds[2];
	int **fd;
	
	fd = ft_pip(cmd_num);
	if (cmd->red_in)
		fds[0] = open(cmd->red_in, O_RDONLY);
	if (cmd->red_out)
		fds[1] = open(cmd->red_out, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fds[0] == -1)
		return(false);
	ft_stdin(fd, pos, fds);
	ft_stdout(cmd_num, fd, pos, fds);
	ft_close(cmd_num, fd, fds);
}
extern char **environ;

bool ft_exic(t_cmd *cmds)
{
	int cmd_num;
	int i;
	int *pids;

	cmd_num = ft_count_cmd(cmds);
	pids = malloc(sizeof(int) * cmd_num);
	i = 0;
	if (!pids)
		return (false);
	while (i < cmd_num || cmds != NULL)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (free(pids),false);
		if (pids[i] == 0)
		{
			fd_hindler(cmds, cmd_num, i);
			if (execve(cmds->path, cmds->args, environ) == -1)
				return (false);
		}
		cmds = cmds->pipe;
	}
	return (true);
}