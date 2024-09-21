/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:20:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/21 17:33:23 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *content, int i, t_type type, t_state state)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
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

t_token	*last_type(t_token *token)
{
	t_token	*last;

	last = token;
	while (last->next)
	{
		last = last->next;
	}
	return (last);
}

void	token_add_back(t_data *data, t_token *new)
{
	t_token	*last;

	if (!data && !new)
		return ;
	if (!new)
	{
		free_tokens(&data->tokens);
		exit(1);
	}
	if (!data->tokens)
	{
		data->tokens = new;
		return ;
	}
	last = last_type(data->tokens);
	last->next = new;
	new->prev = last;
}
