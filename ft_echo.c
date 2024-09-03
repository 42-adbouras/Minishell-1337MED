/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:18:21 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 17:36:40 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_echo(t_exec *cmd, int fd_out)
{
	int	i;

	i = echo_option(cmd) - 1;
	while (cmd->path_option_args[i + 1] && cmd->path_option_args[++i])
	{
		ft_putstr_fd(cmd->path_option_args[i], fd_out);
		if (cmd && cmd->path_option_args[i] && cmd->path_option_args[i + 1])
			ft_putstr_fd(" ", fd_out);
	}
	if (echo_option(cmd) - 1 == 0)
		ft_putstr_fd("\n", fd_out);
	g_status = 0;
	return (true);
}

int	echo_option(t_exec *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	if (!cmd->path_option_args)
		return (0);
	while (cmd->path_option_args[++i])
	{
		j = 0;
		if (cmd->path_option_args[i][0] == '-')
		{
			while (cmd->path_option_args[i][++j])
			{
				if (cmd->path_option_args[i][j] != 'n')
					return (i);
			}
		}
		else
			break ;
	}
	return (i);
}
