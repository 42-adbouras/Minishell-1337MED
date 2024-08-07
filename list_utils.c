/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:20:15 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/07 18:31:14 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_elem	*new_token(char *content, int i, t_token type, t_state state)
{
	t_elem	*new;

	new = malloc(sizeof(t_elem));
	if (!new)
		printf("malloc!\n");
	new->content = ft_strndup(content, i);
	new->len = i;
	new->type = type;
	new->state = state;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_elem	*last_token(t_elem *token)
{
	t_elem	*last;

	last = token;
	while (last->next)
	{
		last = last->next;
	}
	return (last);
}
void	token_add_back(t_data *token, t_elem *new)
{
	if (!token && !new)
		return ;
	if (!token->head)
		token->head = new;
	else
	{
		token->tail->next = new;
		new->prev = token->tail;
	}
	token->tail = new;
	token->size++;
}
