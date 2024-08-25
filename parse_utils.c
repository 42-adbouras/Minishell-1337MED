/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/23 10:15:48 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool redirect_out(t_elem *tokens,t_exec ***new, t_env *env)
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
	int i=0;
	int j=0;
	int l=0;
	
	temp = tokens;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIR_IN && (temp->next && temp->next->state == GENERAL))
		{
			(*new)->heredoc = false;
			(*new)->redir_in[i++] = get_redire(&temp, env);
			if (!(*new)->redir_in[i - 1])
				return (false);
		}
		else if ((temp->type == REDIR_OUT || temp->type == REDIR_APP) && (temp->next && temp->next->state == GENERAL))
		{
			(*new)->append = false;
			if (temp->type == REDIR_APP)
				(*new)->append = true;
			(*new)->redir_out[j++] = get_redire(&temp, env);
			if (!(*new)->redir_out[j - 1])
				return (false);
		}
		else if (temp->type == REDIR_AND && (temp->next && temp->next->state == GENERAL))
		{
			(*new)->heredoc_end[l++] = get_heredoc(&temp);
			(*new)->heredoc = last_heredoc(temp);
		}
		temp = temp->next;
	}
	return (true);
}

void	process_expander(t_elem **temp, t_exec **new, t_env *env, int *i)
{
	if ((*temp)->next)
	{
		(*temp) = (*temp)->next;
		if ((*temp)->content[0] == '?')
			(*new)->path_option_args[(*i)++] = ft_strjoin(ft_itoa(g_status), &((*temp)->content[1]));
		else if ((*temp)->type == WORD)
			(*new)->path_option_args[(*i)++] = ft_expand(env, (*temp)->content);
		else
			(*new)->path_option_args[(*i)++] = ft_strdup("$");
	}
	else
		(*new)->path_option_args[(*i)++] = ft_strdup("$");
}

char *ft_expand(t_env *env, char *var)
{
	char *s;
	char *after;
	t_env *temp;
	int i;
	int j;
	int l;

	i = 0;
	j = 0;
	while (var[i])
	{
		temp = env; 
		j = i;
		l = 0;
		while (var[j] && !ft_isalnum(var[j++]))
			i++;
		s = ft_substr(var, l, j);
		l = j;
		while (var[l] && !ft_isalnum(var[l++]))
		;
		after = ft_substr (var, j, l);
		while (temp)
		{
			if (!ft_strncmp(temp->var, s, ft_strlen(temp->var) + 1))
				return (ft_strjoin(temp->value, after));
			temp = temp->next;
		}
		i++;
	}
	return (NULL);
}
char	*get_arg(t_elem **token, t_env *env)
{
	char	*arg;
	t_state	state;

	arg = NULL;
	if (!*token)
		return (NULL);
	(*token) = (*token)->next;
	state = (*token)->state;
	while ((*token) && (*token)->state == state)
	{
		if ((*token) && (*token)->type == ENV && (*token)->state == IN_DQUOTE)
		{
			(*token) = (*token)->next;
			if ((*token) && (*token)->type == WORD)
				arg = ft_strjoin(arg, ft_expand(env, (*token)->content));
			else
				arg = ft_strjoin(arg,ft_strdup("$"));
		}
		else
			arg = ft_strjoin(arg, (*token)->content);
		(*token) = (*token)->next;
	}
	return (arg);
}
