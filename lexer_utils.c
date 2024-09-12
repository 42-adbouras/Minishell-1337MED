/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:19:27 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 12:46:12 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_red(t_type type)
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

t_token	*skip_wspace(t_token *token, char direction)
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
