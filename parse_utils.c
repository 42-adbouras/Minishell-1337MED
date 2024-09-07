/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/07 15:39:46 by eismail          ###   ########.fr       */
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
	if ((token) && (*token))
		(*token) = (*token)->next;
	if ((*token) && ((*token)->type == D_QUOTE
			|| (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
	{
		(*token) = (*token)->next;
		if ((*token) && ((*token)->type == D_QUOTE
				|| (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
			(*token) = (*token)->next;
		if ((*token) && (((*token)->type != W_SPACE && !is_red((*token)->type)
					&& (*token)->type != PIPE) || (*token)->state != GENERAL))
		{
			*state = (*token)->state;
			if ((*token)->type != WORD && (*token)->type != ENV
				&& !is_red((*token)->type) && (*token)->type != PIPE
				&& (*token)->state == GENERAL)
				(*token) = (*token)->next;
		}
	}
}

char	*get_arg(t_elem **t, t_env *env, bool exec)
{
	char	*arg;
	char	*join;
	t_state	state;

	arg = NULL;
	join = ft_strdup("");
	skip_quotes(&t, &state);
	while ((*t) && ((*t)->state == state))
	{
		if ((*t) && (*t)->type == ENV && env && (*t)->state != IN_SQUOTE)
		{
			(*t) = (*t)->next;
			arg = arg_expand(*t, env, &arg);
		}
		else
			arg = arg_join(*t, &arg, join);
		if ((*t) && (*t)->type == ENV && env && (*t)->state != IN_SQUOTE)
			continue ;
		rest_function(t, &state);
		if ((*t) && (((*t)->type == W_SPACE
					|| (*t)->type == PIPE) && (*t)->state == GENERAL))
			break ;
	}
	return (check_exec(exec, &arg, &join, env));
}

void	init_exec_node(t_exec **new, t_elem *tokens, t_env *env)
{
	int	n;
	int	out;

	n = count_words(tokens);
	out = (count_red(tokens, REDIR_OUT) + count_red(tokens, REDIR_APP));
	(*new)->path_option_args[n] = NULL;
	(*new)->redir_in[count_red(tokens, REDIR_IN)] = NULL; 
	(*new)->redir_out[out] = NULL; 
	(*new)->heredoc_end[count_red(tokens, REDIR_AND)] = NULL;
	(*new)->env = env;
	(*new)->exed = false;
	(*new)->append = false;
	(*new)->heredoc = false;
	(*new)->ambiguous = false;
	(*new)->expand_heredoc = false;
	(*new)->next = NULL;
}
