/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:35:09 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
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
