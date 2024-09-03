/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_count_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:57:01 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 11:28:42 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	not_pipe(t_elem *tokens)
{
	if ((tokens && tokens->type != PIPE)
		|| (tokens && (tokens)->type == PIPE && (tokens)->state != GENERAL))
		return (true);
	return (false);
}

bool	should_skip(t_elem *tokens)
{
	if (((tokens->prev && tokens->prev->type == PIPE
				&& tokens->prev->state == GENERAL))
		|| (tokens && tokens->type == W_SPACE && tokens->state == GENERAL))
		return (true);
	return (false);
}

bool	is_word(t_elem *tokens)
{
	if (!tokens->next || (tokens->next && (tokens->next->type == W_SPACE
				|| tokens->next->type == PIPE)
			&& tokens->next->state == GENERAL))
		return (true);
	return (false);
}

bool	increment(t_elem *tokens)
{
	if ((tokens && tokens->type != PIPE)
		|| (tokens && (tokens)->type == PIPE && (tokens)->state != GENERAL))
		return (true);
	return (false);
}
