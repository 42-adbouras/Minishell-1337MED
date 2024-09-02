/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:15:27 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 13:43:55 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd_getter(t_elem *temp, t_exec *new)
{
	if (temp->type == WORD && !new->exed && temp->next
		&& (temp->next->type != S_QUOTE && temp->next->type != D_QUOTE))
		return (true);
	return (false);
}

bool	arg_getter(t_elem *temp)
{
	if (temp && ((temp->type == S_QUOTE || temp->type == D_QUOTE)
			|| (temp->type == WORD && temp->state == GENERAL)))
		return (true);
	return (false);
}
