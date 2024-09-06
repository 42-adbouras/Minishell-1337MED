/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:50:36 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/06 21:24:04 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_swipe(char **join, char **arg)
{
	char	*join2;

	join2 = ft_strjoin(*join, *arg);
	if (join && *join)
	{
		free(*join);
		*join = NULL;
	}
	if (arg && *arg)
	{
		free(*arg);
		*arg = NULL;
	}
	*arg = ft_strdup(join2);
	free(join2);
}

char	*ft_expander(t_elem **temp, t_env *env, bool exec)
{
	char	*arg;
	char	*join;

	arg = NULL;
	join = NULL;
	while (temp && *temp && ((*temp)->type == ENV
			|| (*temp)->type == D_QUOTE || (*temp)->type == S_QUOTE))
	{
		join = ft_strdup(arg);
		free(arg);
		arg = NULL;
		if (*temp && ((*temp)->type == D_QUOTE || (*temp)->type == S_QUOTE))
			arg = get_arg(temp, env, true);
		else if ((*temp) && (*temp)->next)
		{
			(*temp) = (*temp)->next;
			arg = arg_expand(*temp, env, &arg);
		}
		else
			arg = ft_strdup("$");
		ft_swipe(&join, &arg);
		if (*temp && (*temp)->type != W_SPACE && (*temp)->state == GENERAL)
			*temp = (*temp)->next;
	}
	return (check_exec(exec, &arg, &join, env));
}

char	*arg_expand(t_elem *token, t_env *env, char **arg)
{
	char	*temp;
	char	*join;
	char	*join2;
	char	*nb;

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
	return (free(join), free(temp), join2);
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

char	*ft_expand(t_env *env, char *var)
{
	t_env	*temp;
	char	*after;
	char	*variable;
	char	*re;

	temp = env;
	re = NULL;
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
	if (after && !re)
		return (free(variable), after);
	return (free(after), free(variable), NULL);
}

char	*get_after(char *var)
{
	char	*after;
	size_t		i;
	int		count;

	i = 0;
	count = 0;
	after = NULL;
	if (var[0] && !ft_isalpha(var[0]) && var[0] != '_')
	{
		after = ft_substr(var, 1, ft_strlen(var));
		return (after);
	}
	while (var[i] && (ft_isalnum(var[i]) || var[i] == '_'))
	{
		i++;
	}
	if (i > 0 )
		after = ft_substr(var, i, ft_strlen(var));
	return (after);
}
