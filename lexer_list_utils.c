/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:20:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/29 10:32:09 by eismail          ###   ########.fr       */
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

void	token_add_back(t_data *tokens, t_elem *new)
{
	t_elem	*last;

	if (!tokens && !new)
		return ;
	if (!tokens->head)
	{
		tokens->head = new;
		return ;
	}
	last = last_token(tokens->head);
	last->next = new;
	new->prev = last;
	tokens->size++;
}
