/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 12:57:02 by adbouras         ###   ########.fr       */
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

bool	get_redir_in(t_exec ***new, t_elem *temp, t_env *env, int i)
{
	if (temp->type == REDIR_IN && temp->state == GENERAL)
	{
		(**new)->heredoc = false;
		(**new)->redir_in[i] = get_redire(&temp, env);
		if ((new) && (*new) && (**new)
			&& (**new)->redir_out && !(**new)->redir_in[i])
			return (false);
	}
	return (true);
}

bool	get_redir_out(t_exec ***new, t_elem *temp, t_env *env, int j)
{
	(**new)->append = false;
	if (temp->type == REDIR_APP)
		(**new)->append = true;
	(**new)->redir_out[j] = get_redire(&temp, env);
	if ((**new) && (**new)->redir_out && !(**new)->redir_out[j])
		return (false);	
	return (true);
}

void	heredoc_getter(t_exec ***new, t_elem *temp, int l)
{
	(**new)->heredoc_end[l] = get_heredoc(&temp);
	(**new)->heredoc = last_heredoc(temp);
}

bool	redir_conditions(t_elem *temp, int flag)
{
	if (flag == 0)
	{
		if (temp && (temp->type != PIPE || ((temp->type == PIPE && temp->state != GENERAL))))
			return (true);
		else
			return (false);
	}
	else if (flag == 1)
	{
		if ((temp->type == REDIR_OUT || temp->type == REDIR_APP) && temp->state == GENERAL)
			return (true);
		else
			return (false);
	}
	else if (flag == 2)
	{
		if (temp && (temp->type != PIPE || (temp->type == PIPE && temp->state != GENERAL)))
			return (true);
		else
			return (false);
	}
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
		if (!get_redir_in(&new, temp, env, i++))
			return (false);
		else if (redir_conditions(temp, 1))
		{
			if (!get_redir_out(&new, temp, env, j++))
				return (false);
		}
		else if (temp->type == REDIR_AND && temp->state == GENERAL)
			heredoc_getter(&new, temp, l++);
		if (redir_conditions(temp, 2))
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
	char	*after;
	int		i;
	
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

char	*ft_expand(t_env *env, char *var)
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
		if((*token) && ((*token)->type == D_QUOTE || (*token)->type == S_QUOTE) && (*token)->state == GENERAL)
			(*token) = (*token)->next;
		if ((*token) && (*token)->type != W_SPACE && !is_red((*token)->type) && (*token)->type != PIPE)
		{
			*state = (*token)->state;
			if ((*token)->type != WORD)
				(*token) = (*token)->next;
		}
	}
}

char	*arg_expand(t_elem *token, t_env *env, char **arg)
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

char	*arg_join(t_elem *token, char **arg, char *join)
{
	char *join2;

	join2 = NULL;
	if (token && (token->type == D_QUOTE || token->type == S_QUOTE) && token->state == GENERAL)
		join2 = ft_strjoin(*arg, join);
	else
		join2 = ft_strjoin(*arg, (token)->content);
	free(*arg);
	return (join2);
}

void	skip_quotes(t_elem ***token, t_state *state)
{
	if ((**token) && ((**token)->type == D_QUOTE || (**token)->type == S_QUOTE))
		(**token) = (**token)->next;
	if (**token)
		*state = (**token)->state;
}

char	*check_exec(bool exec, char **arg, char **join, t_env *env)
{
	char	*res;

	res = NULL;
	if (!exec && !if_builtin(*arg))
	{
		res = get_access(*arg, env);
		if (ft_strlen(*arg) <= ft_strlen(res))
			free(*arg);
		return (free(*join), res);
	}
	if (!res)
		return (free(*join), *arg);
	return (res);
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
		_function(token, &state);
		if ((*token) &&  (((*token)->type == W_SPACE
			|| (*token)->type == PIPE) && (*token)->state == GENERAL))
			break;
	}
	return (check_exec(exec, &arg, &join, env));
}
