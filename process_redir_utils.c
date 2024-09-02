/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:27:24 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 15:29:16 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	redir_conditions(t_elem *temp, int flag)
{
	if (flag == 0)
	{
		if (temp && (temp->type != PIPE
				|| ((temp->type == PIPE && temp->state != GENERAL))))
			return (true);
	}
	else if (flag == 1)
	{
		if ((temp->type == REDIR_OUT || temp->type == REDIR_APP)
			&& temp->state == GENERAL)
			return (true);
	}
	else if (flag == 2)
	{
		if (temp && (temp->type != PIPE
				|| (temp->type == PIPE && temp->state != GENERAL)))
			return (true);
	}
	return (false);
}

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
	if (temp->type == REDIR_IN && temp->state == GENERAL)
	{
		(**new)->heredoc = false;
		(**new)->redir_in[(*i)++] = get_redire(&temp, env);
		if ((new) && (*new) && (**new)
			&& (**new)->redir_out && !(**new)->redir_in[(*i) - 1])
			return (false);
	}
	return (true);
}

bool	get_redir_out(t_exec ***new, t_elem *temp, t_env *env, int *j)
{
	(**new)->append = false;
	if (temp->type == REDIR_APP)
		(**new)->append = true;
	(**new)->redir_out[(*j)++] = get_redire(&temp, env);
	if ((**new) && (**new)->redir_out && !(**new)->redir_out[(*j) - 1])
		return (false);
	return (true);
}

void	heredoc_getter(t_exec ***new, t_elem *temp, int *l)
{
	(**new)->heredoc_end[(*l)++] = get_heredoc(&temp);
	(**new)->heredoc = last_heredoc(temp);
}
