/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:19:27 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/21 19:39:01 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_red(t_token type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APP || type == REDIR_AND)
		return (true);
	return (false);
}

bool	is_white_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f')
		return (true);
	return (false);
}

bool	is_grammar(char c)
{
	if (c == '\'' || c == '\"' || c == '<' || c == '>'
		|| c == '|' || c == '$' || is_white_space(c))
		return (true);
	return (false);
}

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

t_elem	*skip_wspace(t_elem *token, char direction)
{
	
	while (token && token->type == W_SPACE)
	{
		if (direction == 'N')
			token = token->next;
		else if (direction == 'P')
			token = token->prev;
		else
			return (NULL);
	}
	return (token);
}