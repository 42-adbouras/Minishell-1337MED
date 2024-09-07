/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:02:41 by eismail           #+#    #+#             */
/*   Updated: 2024/09/07 15:49:51 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_skip(int *index, char *delimi, char *temp)
{
	if (!ft_strncmp(temp, delimi, ft_strlen(delimi) + 1))
		return (true);
	while (temp[(*index)] && temp[(*index)] != '$')
		(*index)++;
	if (temp[(*index)] && temp[(*index)] != '$')
		(*index)--;
	return (false);
}

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

bool	ft_path(t_env *env)
{
	while (env)
	{
		if ((ft_strncmp(env->var, "PATH", 5) == 0) && env->value != NULL)
			return (true);
		env = env->next;
	}
	return (false);
}

bool	init_fds(int **pids, t_fd **fd, int cmd_num)
{
	*pids = malloc(sizeof(int) * cmd_num);
	*fd = malloc(sizeof(t_fd));
	(*fd)->fds = malloc(sizeof(int *) * cmd_num);
	(*fd)->pipes = ft_pip(cmd_num, true);
	if (!(*fd)->pipes)
	{
		ft_error("Resource temporarily unavailable\n", 1);
		free(*pids);
		free(*fd);
		return (false);
	}
	return (true);
}

void	ft_exic(t_exec *cmds, t_env **env)
{
	int		cmd_num;
	int		i;
	int		*pids;
	t_fd	*fd;

	cmd_num = ft_count_cmd(cmds);
	i = -1;
	if (!init_fds(&pids, &fd, cmd_num))
		return ;
	while (++i < cmd_num)
	{
		fd->fds[i] = ft_open(cmds);
		if (skip_cmd(&cmds, fd->fds[i]))
			continue ;
		if (run_one_builtin(cmds, &env, fd, cmd_num))
			break ;
		pids[i] = fork();
		if (pids[i] == -1)
			return (free_fds(pids, &fd, cmd_num));
		if (pids[i] == 0)
			ft_run_cmd(cmds, &env, fd, i);
		cmds = cmds->next;
	}
	ft_clear(cmd_num, fd, pids);
}
