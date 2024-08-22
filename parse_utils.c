/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/22 17:10:41 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_redir(t_elem *tokens, t_exec **new)
{
	t_elem	*temp;
	
	temp = tokens;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIR_IN )
		{
			(*new)->heredoc = false;
			*((*new)->redir_in)++ = get_redirec(&temp);
		}
		else if (temp->type == REDIR_OUT || temp->type == REDIR_APP)
		{
			(*new)->append = false;
			if (temp->type == REDIR_APP)
				(*new)->append = true;
			*((*new)->redir_out)++ = get_redirec(&temp);
		}
		else if (temp->type == REDIR_AND)
		{
			*((*new)->heredoc_end)++ = get_redirec(&temp);
			(*new)->heredoc = last_heredoc(temp);
		}
		temp = temp->next;
	}
}

void	process_expander(t_elem *temp, t_exec **new, t_env *env, int *i)
{
	if (temp->next)
	{
		temp = temp->next;
		if (temp->type == WORD)
			(*new)->path_option_args[(*i)++] = ft_expand(env, temp->content);
		if (temp->content[ft_strlen(temp->content) - 1] == '=')
        {
            while (temp && temp->type != S_QUOTE && temp->type != D_QUOTE)
                temp = temp->next;
            (*new)->path_option_args[(*i) - 1] = ft_strjoin((*new)->path_option_args[(*i) - 1], get_arg(&temp, env));
        }
		if (temp->content[0] == '-')
        {
            while (temp && temp->type != S_QUOTE && temp->type != D_QUOTE)
                temp = temp->next;
            (*new)->path_option_args[(*i) - 1] = ft_strjoin((*new)->path_option_args[(*i) - 1], get_arg(&temp, env));
        }
		else
			(*new)->path_option_args[(*i)++] = ft_strdup("$");
	}
	else
		(*new)->path_option_args[(*i)++] = ft_strdup("$");
}

char *ft_expand(t_env *env, char *var)
{
	while(env)
	{
		if (!ft_strncmp(env->var, var, ft_strlen(env->var) + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
char	*get_arg(t_elem **token, t_env *env)
{
	char	*arg;
	t_state	state;

	arg = NULL;
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