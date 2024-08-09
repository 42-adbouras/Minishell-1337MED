/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:19:27 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/09 11:38:48 by adbouras         ###   ########.fr       */
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
