/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:35:09 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/06 21:27:11 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_pwd(int fd_out)
{
	char	s[MAX_PATH];

	if (getcwd(s, MAX_PATH) == NULL)
	{
		g_status = 1;
		perror("minishell: ");
		return (false);
	}
	ft_putstr_fd(s, fd_out);
	ft_putstr_fd("\n", fd_out);
	g_status = 0;
	return (true);
}
