/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:16:48 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/18 11:31:59 by eismail          ###   ########.fr       */
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
	if (prev->type != WORD && prev->type != D_QUOTE && prev->type != S_QUOTE) // double QUOTE before pip
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
				return (ft_error("[syntax error detected unclosed ' or \"]\n"));
		}
		if (is_red(tmp->type))
		{
			if (!red_syntax(tmp))
				return (ft_error(
						"[syntax error near unexpected token `newline']\n"));
		}
		if (tmp->type == PIPE)
		{
			if (!pipe_syntax(tmp))
				return (ft_error("[syntax error near unexpected token `|']\n"));
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
