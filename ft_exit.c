/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:33:36 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 16:11:23 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit_error(char c)
{
	if (!ft_isdigit(c))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		exit (255);
	}
}

bool	ft_exit_built(char **arg, int cmd_num)
{
	int	i;

	i = -1;
	if (!arg || !*arg)
	{
		if (cmd_num > 1)
			exit(0);
		exit(g_status);
	}
	while (arg && *arg && arg[0][++i])
	{
		if (arg[0][0] == '-')
			i++;
		ft_exit_error(arg[0][i]);
	}
	if (arg && *arg && arg[1])
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (false);
	}
	else
		exit(ft_atoi(arg[0]));
}
