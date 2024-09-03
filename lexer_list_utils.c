/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:20:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 10:10:26 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_elem	*new_token(char *content, int i, t_token type, t_state state)
{
	t_elem	*new;

	new = malloc(sizeof(t_elem));
	if (!new)
	{
		ft_error("malloc failed!\n", 1);
		return (NULL);
	}
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
	if (!new)
	{
		free_tokens(&tokens->head);
		exit(1);
	}
	if (!tokens->head)
	{
		tokens->head = new;
		return ;
	}
	last = last_token(tokens->head);
	last->next = new;
	new->prev = last;
}
