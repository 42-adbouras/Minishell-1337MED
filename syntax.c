/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:16:48 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/09 11:40:25 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	pipe_syntax(t_elem *token)
{
	t_elem	*next;
	t_elem	*prev;

	next = skip_wspace(token->next, 'N');
	prev = skip_wspace(token->prev, 'P');
	if (!next || !prev)
		return (false);
	if (next->type != WORD && !is_red(next->type))
		return (false);
	if (prev->type != WORD)
		return (false);
	return (true);
}

bool	if_syntax_err(t_data *tokens)
{
	t_elem	*tmp;

	tmp = tokens->head;
	while (tmp)
	{
		if (tmp->type == D_QUOTE || tmp->type == S_QUOTE)
		{
			if (!if_closed_quotes(&tmp, tmp->type))
			{
				printf("UNCLOSED QUOTES\n");
				return (true);
			}
		}
		if (is_red(tmp->type))
		{
			if (!red_syntax(tmp))
			{
				printf("REDIRECTION SYNTAX ERR\n");
				return (true);
			}
		}
		if (tmp->type == PIPE)
		{
			if (!pipe_syntax(tmp))
			{
				printf("PIPE SYNTAX ERR\n");
				return (true);
			}
		}
		tmp = tmp->next;
	}
	return (false);
}

bool	if_closed_quotes(t_elem **token, t_token type)
{
	while ((*token))
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == type)
			return (true);
	}
	return (false);
}

bool	red_syntax(t_elem *token)
{
	t_elem	*next;

	next = skip_wspace(token->next, 'N');
	if (!next || next->type != WORD)	// maybe missing conditions!
		return (false);
	return (true);
}
