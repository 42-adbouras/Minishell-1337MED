/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:15:27 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/07 15:46:18 by eismail          ###   ########.fr       */
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

char	*get_exit_status(char *str)
{
	char	*nb;
	char	*join;
	char	*temp;

	join = NULL;
	nb = ft_itoa(g_status);
	if (ft_strlen(str) > 1)
		join = ft_strdup(&str[1]);
	temp = ft_strjoin(nb, join);
	free(join);
	free(nb);
	return (temp);
}
