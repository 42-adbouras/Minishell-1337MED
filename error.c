/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:05:46 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_error(char *err, int exit)
{
	write(2, err, ft_strlen(err));
	g_status = exit;
	return (1);
}

void	ft_exit(t_data **tokens, char *err)
{
	free_tokens(&(*tokens)->tokens);
	write(2, err, ft_strlen(err));
	if (err)
		exit(1);
	exit(0);
}
