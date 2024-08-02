/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:24:58 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/31 20:48:31 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	_leaks(void)
{
	system("leaks minishell");
}

void	node_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (!(*lst)->cmd)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->pipe)
		last = last->pipe;
	last->pipe = new;
}

t_cmd	*new_node(char *content)
{
	t_cmd	*node;

	node = malloc (sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->cmd = content;
	node->path = NULL;
	node->rl = NULL;
	node->option = NULL;
	node->split = NULL;
	node->red_in = NULL;
	node->red_out = NULL;
	node->pipe = NULL;
	return (node);
}
