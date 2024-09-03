/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:24:07 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 17:24:21 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_env(t_env *env, int fd_out)
{
	while (env)
	{
		if (ft_strchr(env->value, '='))
		{
			ft_putstr_fd(env->var, fd_out);
			ft_putstr_fd(env->value, fd_out);
			ft_putstr_fd("\n", fd_out);
		}
		env = env->next;
	}
	return (true);
}
