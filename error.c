/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:05:46 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 19:02:43 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_error(char *err)
{
	// write(2, BRED, ft_strlen(BRED));
	write(2, err, ft_strlen(err));
	// write(2, RSET, ft_strlen(RSET));
	g_status = 258;
	return (EXIT_FAILURE);
}

void	ft_exit(t_data **tokens, char *err)
{
	free_tokens(&(*tokens)->head);
	write(2, err, ft_strlen(err));
	if (err)
		exit(1);
	exit(0);
}
