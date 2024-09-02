/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:33:36 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 17:33:56 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (!ft_isdigit(arg[0][i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			exit (255);
		}
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
