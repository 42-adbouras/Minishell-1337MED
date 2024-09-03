/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:27:24 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 11:46:55 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_redir(t_elem **token)
{
	(*token) = (*token)->next;
	while ((*token) && (*token)->type == W_SPACE && (*token)->state == GENERAL)
		(*token) = (*token)->next;
	if ((*token) && (*token)->type == ENV && (*token)->state == GENERAL)
	{
		(*token) = (*token)->next;
		if (*token)
			(*token) = (*token)->next;
	}
	while ((*token) && (((*token)->type != W_SPACE && (*token)->type != PIPE)
			|| (((*token)->type == W_SPACE && (*token)->type == PIPE)
				&& (*token)->state == GENERAL)))
		(*token) = (*token)->next;
}

bool	get_redir_in(t_exec ***new, t_elem *temp, t_env *env, int *i)
{
	bool	ambiguous;

	if (temp->type == REDIR_IN && temp->state == GENERAL)
	{
		ambiguous = false;
		(**new)->heredoc = false;
		(**new)->redir_in[(*i)++] = get_redire(&temp, env, &ambiguous);
		if (ambiguous)
			(**new)->ambiguous = true;
		if ((new) && (*new) && (**new)
			&& (**new)->redir_out && !(**new)->redir_in[(*i) - 1])
		{
			g_status = 1;
			return (false);
		}
	}
	return (true);
}

bool	get_redir_out(t_exec ***new, t_elem *temp, t_env *env, int *j)
{
	bool	ambiguous;

	ambiguous = false;
	(**new)->append = false;
	if (temp->type == REDIR_APP)
		(**new)->append = true;
	(**new)->redir_out[(*j)++] = get_redire(&temp, env, &ambiguous);
	if (ambiguous)
		(**new)->ambiguous = true;
	if ((**new) && (**new)->redir_out && !(**new)->redir_out[(*j) - 1])
	{
		g_status = 1;
		return (false);
	}
	return (true);
}

void	heredoc_getter(t_exec ***new, t_elem *temp, int *l)
{
	bool	heredoc;

	heredoc = false;
	(**new)->heredoc_end[(*l)++] = get_heredoc(&temp, &heredoc);
	(**new)->heredoc = last_heredoc(temp);
	if (heredoc)
		(**new)->expand_heredoc = true;
}
