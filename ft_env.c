/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:24:07 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env(t_env **tokens, t_env *env_new)
{
	t_env	*last;

	if (*tokens == NULL)
		*tokens = env_new;
	else 
	{
		last = ft_last(*tokens);
		last->next = env_new;
	}
}

t_env	*creat_var(char *var)
{
	int		j;
	t_env	*envi;

	envi = malloc(sizeof(t_env));
	j = 0;
	while (var[j] && var[j] != '=')
		j++;
	envi->var = ft_substr(var, 0, j);
	envi->value = ft_substr(var, j, ft_strlen(var));
	envi->next = NULL;
	return (envi);
}

t_env	*set_env(char **env)
{
	int		i;
	t_env	*envi;
	t_env	*new;

	i = -1;
	envi = NULL;
	while (env[++i])
	{
		new = creat_var(env[i]);
		add_env(&envi, new);
	}
	return (envi);
}

bool	ft_env(t_env *env, int fd_out)
{
	while (env)
	{
		if (ft_strchr(env->value, '='))
		{
			ft_putstr_fd(env->var, fd_out);
			ft_putstr_fd(env->value, fd_out);
			ft_putstr_fd("\n", fd_out);
		}
		env = env->next;
	}
	return (true);
}
