/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 14:41:30 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	process_redir(t_elem *tokens, t_exec **new, t_env *env)
{
	t_elem	*temp;
	int		i;
	int		j;
	int		l;

	temp = tokens;
	i = 0;
	j = 0;
	l = 0;
	while (redir_conditions(temp, 0))
	{
		if (!get_redir_in(&new, temp, env, &i))
			return (false);
		else if (redir_conditions(temp, 1))
		{
			if (!get_redir_out(&new, temp, env, &j))
				return (false);
		}
		else if (temp->type == REDIR_AND && temp->state == GENERAL)
			heredoc_getter(&new, temp, &l);
		if (redir_conditions(temp, 2))
			temp = temp->next;
	}
	return (true);
}

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

void	rest_function(t_elem **token, t_state *state)
{
	(*token) = (*token)->next;
	if ((*token) && ((*token)->type == D_QUOTE
			|| (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
	{
		(*token) = (*token)->next;
		if ((*token) && ((*token)->type == D_QUOTE
				|| (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
			(*token) = (*token)->next;
		if ((*token) && (*token)->type != W_SPACE
			&& !is_red((*token)->type) && (*token)->type != PIPE)
		{
			*state = (*token)->state;
			if ((*token)->type != WORD)
				(*token) = (*token)->next;
		}
	}
}

void	skip_quotes(t_elem ***token, t_state *state)
{
	if ((**token) && ((**token)->type == D_QUOTE || (**token)->type == S_QUOTE))
		(**token) = (**token)->next;
	if (**token)
		*state = (**token)->state;
}

char	*get_arg(t_elem **token, t_env *env, bool exec)
{
	char	*arg;
	char	*join;
	t_state	state;

	arg = NULL;
	join = ft_strdup("");
	skip_quotes(&token, &state);
	while ((*token) && ((*token)->state == state))
	{
		if ((*token) && (*token)->type == ENV && ((*token)->state == IN_DQUOTE || (*token)->state == GENERAL))
		{
			(*token) = (*token)->next;
			arg = arg_expand(*token, env, &arg);
		}
		else
			arg = arg_join(*token, &arg, join);
		if ((*token) && (*token)->type == ENV && ((*token)->state == IN_DQUOTE || (*token)->state == GENERAL))
			continue ;
		rest_function(token, &state);
		if ((*token) && (((*token)->type == W_SPACE
					|| (*token)->type == PIPE) && (*token)->state == GENERAL))
			break ;
	}
	return (check_exec(exec, &arg, &join, env));
}
