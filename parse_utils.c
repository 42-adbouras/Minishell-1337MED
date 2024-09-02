/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 16:46:36 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	redirect_out(t_elem *tokens, t_exec ***new, t_env *env)
{
	(*(*new))->append = false;
	if (tokens->type == REDIR_APP)
		(*(*new))->append = true;
	*((*(*new))->redir_out)++ = get_redire(&tokens, env);
	if ((*(*new))->redir_in)
		return (false);
	return (true);
}

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

void	rest_function(t_elem **token, t_state *state)
{
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
		if ((*token) && (*token)->type == ENV && (*token)->state == IN_DQUOTE)
		{
			(*token) = (*token)->next;
			arg = arg_expand(*token, env, &arg);
		}
		else
			arg = arg_join(*token, &arg, join);
		(*token) = (*token)->next;
		rest_function(token, &state);
		if ((*token) && (((*token)->type == W_SPACE
					|| (*token)->type == PIPE) && (*token)->state == GENERAL))
			break ;
	}
	return (check_exec(exec, &arg, &join, env));
}
