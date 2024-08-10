/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:05:46 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/10 12:02:49 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void ft_error(char *err)
{
	write(2, BRED, ft_strlen(BRED));
	write(2, err, ft_strlen(err));
	write(2, RSET, ft_strlen(RSET));
}

void	ft_exit(t_data **tokens, char *err)
{
	free_tokens(tokens);
	write(2, err, ft_strlen(err));
	if (err)
		exit(1);
	exit(0);
}
