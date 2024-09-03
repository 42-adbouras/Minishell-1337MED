/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exic_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:11 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 15:36:37 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_cmd(t_exec *cmd, t_env ***env, t_fd *fd, int i)
{
	char	**strenv;
	int		cmd_num;

	cmd_num = ft_count_cmd(cmd);
	signal(SIGQUIT, child_signal);
	if (cmd_num > 0)
		cmd_num += i;
	strenv = env_to_str(**env);
	if (g_status == 300)
		exit(1);
	fd_hindler(cmd_num, fd->pipes, fd->fds, i);
	if (cmd->path_option_args && if_builtin(cmd->path_option_args[0]))
	{
		if (if_builtin(cmd->path_option_args[0]) && !ft_builtin(cmd, *env, 1))
			exit(1);
		else
			exit(0);
	}
	else if (execve(cmd->path_option_args[0],
			cmd->path_option_args, strenv) == -1)
	{
		free_char_arr(strenv);
		ft_exec_error(cmd);
	}
}

bool	run_one_builtin(t_exec *cmds, t_env ***env, t_fd *fd, int cmd_num)
{
	if (cmd_num == 1 && if_builtin(cmds->path_option_args[0]))
	{
		if (!ft_builtin(cmds, *env, fd->fds[1]))
			g_status = 1;
		close_fds(&fd);
		return (true);
	}
	return (false);
}

void	ft_print_error(char *message, int exit_status)
{
	if (!message)
		perror("minishell");
	else
		ft_putstr_fd(message, 2);
	exit(exit_status);
}

void	ft_exec_error(t_exec *cmd)
{
	if (is_directory(cmd->path_option_args[0]) == 1)
		ft_print_error("minishell: is a directory\n", 126);
	else if (errno == EACCES)
		ft_print_error(NULL, 126);
	else if (!ft_path(cmd->env))
		ft_print_error("minishell :No such file or directory\n", 127);
	else if (errno == ENOENT)
		ft_print_error("minishell :command not found\n", 127);
	else
		ft_print_error(NULL, 1);
}

int	ft_count_cmd(t_exec *cmd)
{
	int		i;
	t_exec	*tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
