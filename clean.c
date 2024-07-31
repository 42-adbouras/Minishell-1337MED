/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:47:33 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/31 09:23:57 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}


void	clear_nodes(t_cmd **list)
{
	t_cmd	*tmp;
	
	if (!*list)
		return ;
	while (*list)
	{
		tmp = (*list)->pipe;
		(*list)->cmd = NULL;
		free(*list);
		*list = tmp;
	}
	*list = NULL;
}

void	ft_exit(t_cmd *data)
{
	free (data);
	exit(1);
}
