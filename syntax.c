/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:16:48 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/01 19:37:24 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	pipe_syntax(t_elem *token)
{
	t_elem	*next;
	t_elem	*prev;

	next = skip_wspace(token->next, 'N');
	prev = skip_wspace(token->prev, 'P');
	if (!next || !prev)
		return (false);
	if (next->type != WORD && !is_red(next->type)
		&& prev->type != D_QUOTE && prev->type != S_QUOTE)
		return (false);
	if (prev->type != WORD && prev->type != D_QUOTE && prev->type != S_QUOTE)
		return (false);
	return (true);
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
	if (!next || (next->type != WORD && next->type != ENV
			&& next->type != S_QUOTE && next->type != D_QUOTE))
		return (false);
	return (true);
}
