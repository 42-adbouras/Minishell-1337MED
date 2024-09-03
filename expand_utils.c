/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:50:36 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 13:29:39 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_expander(t_elem **temp, t_env *env, bool exec)
char	*process_expander(t_elem **temp, t_env *env, bool exec)
{
	char	*arg;
	char	*join;
	char	*join2;

	arg = NULL;
	while (temp && *temp && (*temp)->type == ENV)
	{
		join = ft_strdup(arg);
		free(arg);
		arg = NULL;
		if ((*temp)->next)
		{
			(*temp) = (*temp)->next;
			arg = arg_expand(*temp, env, &arg);
		}
		else
			arg = ft_strdup("$");
		join2 = ft_strjoin(join, arg);
		free(arg);
		arg = ft_strdup(join2);
		free(join2);
		// free(join);
		// free(join);
		*temp = (*temp)->next;
	}
	return (check_exec(exec, &arg, &join, env));
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

char	*get_after(char *var)
{
	char	*after;
	int		i;

	i = 0;
	while (var[i] && (var[i] == '_' || ft_isalnum(var[i])))
		i++;
	after = ft_substr(var, i, ft_strlen(var));
	return (after);
}
