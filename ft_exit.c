/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:33:36 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/07 18:07:19 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

bool	ft_exit_error(char c, char *message, bool error)
{
	if (!ft_isdigit(c))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd(message, 2);
		if (error)
			exit (255);
	}
	return (false);
}

void	too_big(char *nbr)
{
	if (ft_strlen(nbr) > 21 || (ft_strlen(nbr) == 20 
			&& ft_strncmp(nbr, "-9223372036854775808", 21) > 0)
		|| ((nbr[0] != '-' && ft_strlen(nbr) == 19 
				&& ft_strncmp(nbr, "9223372036854775807", 20) > 0)))
		ft_exit_error('F', "minishell: exit: numeric argument required\n", true);
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
		too_big(arg[0]);
		if (i == 0 && arg[0][0] == '-')
			i++;
		ft_exit_error(arg[0][i], "minishell: exit: numeric argument required\n",
			true);
	}
	if (arg && *arg && arg[1])
		return (ft_exit_error('F', "minishell: exit: too many arguments\n",
				false));
	else
	{
		ft_putstr_fd("exit\n", 2);
		exit(ft_atoi(arg[0]));
	}
}
