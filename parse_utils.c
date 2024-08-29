/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/29 09:33:52 by eismail          ###   ########.fr       */
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
		(*new)->path_option_args[(*i)] = NULL;
		(*new)->path_option_args[(*i)] = arg_expand(*temp, env, &(*new)->path_option_args[(*i)]);
		(*i)++;
	}
	else
		(*new)->path_option_args[(*i)++] = ft_strdup("$");
}
char *get_after(char *var)
{
	int i;
	char *after;
	
	i = 0;

	while (var[i] && (var[i] == '_' || ft_isalnum(var[i])))
	i++;
	after = ft_substr(var, i, ft_strlen(var));
	return (after);
}
char *get_var(char *var)
{
	int i;
	char *variable;
	
	i = 0;

	while (var[i] && (var[i] == '_' || ft_isalnum(var[i])))
	i++;
	variable = ft_substr(var, 0, i);
	return (variable);
}
char *ft_expand(t_env *env, char *var)
{
	t_env *temp;
	char *after;
	char *variable;
	char *re;
	
	temp = env;
	after = get_after(var);
	variable = get_var(var);
	while (temp)
	{
		if (!ft_strncmp(temp->var, variable, ft_strlen(temp->var) + 1))
		{
			re = ft_strjoin(&temp->value[1], after);
			return (free(after), free(variable), re);
		}
		temp = temp->next;
	}
	return (free(after), free(variable), NULL);
}

void	_function(t_elem **token, t_state *state)
{
	if((*token) && ((*token)->type == D_QUOTE || (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
	{
		(*token) = (*token)->next;
		if((*token) && ((*token)->type == D_QUOTE || (*token)->type == S_QUOTE))
			(*token) = (*token)->next;
		if ((*token) && (*token)->type != W_SPACE && !is_red((*token)->type))
		{
			*state = (*token)->state;
			if ((*token)->type != WORD)
				(*token) = (*token)->next;
		}
	}
}
char *arg_expand(t_elem *token, t_env *env, char **arg)
{
	char *temp;
	char *join;
	char *join2;
	char *nb;
	
	join = NULL;
	temp = *arg;
	if (token && (token)->content[0] == '?')
	{
		nb = ft_itoa(g_status);
		join2 = ft_strjoin(temp, nb);
		free(temp);
		free(nb);
		temp = ft_strdup(join2);
		if (ft_strlen((token)->content) > 1)
			join = ft_strdup(&((token)->content[1]));
		free(join2);
	}
	else if ((token) && (token)->type == WORD)
		join = ft_expand(env, (token)->content);
	else
		join = ft_strdup("$");
	
	join2 = ft_strjoin(temp, join);
	return (free(join), free(*arg), free(temp), join2);
}
char *arg_join(t_elem *token, char **arg, char *join)
{
	char *temp;
	
	temp = *arg;
	if (token && (token->type == D_QUOTE || token->type == S_QUOTE) && token->state == GENERAL)
		temp = ft_strjoin(temp, join);
	else
		temp = ft_strjoin(temp, (token)->content);
	free(*arg);
	return (temp);
}
char	*get_arg(t_elem **token, t_env *env, bool exec)
{
	char	*arg;
	char	*join;
	t_state	state;

	arg = NULL;
	join = ft_strdup("");
	if (!*token)
		return (NULL);
	(*token) = (*token)->next;
	state = (*token)->state;
	while ((*token) && (*token)->state == state)
	{
		if ((*token) && (*token)->type == ENV && (*token)->state == IN_DQUOTE)
		{
			(*token) = (*token)->next;
			arg = arg_expand(*token, env, &arg);
		}
		else
			arg = arg_join(*token, &arg, join);
		(*token) = (*token)->next;
		_function(token, &state);
	}
	if (!exec)
		return (free(join), get_access(arg, env));
	return (free(join), arg);
}
