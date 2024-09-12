/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:26:03 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export(t_env **env, char **arg, int fd_out)
{
	int		i;
	int		j;
	char	*new_var;
	t_env	*new;

	j = -1;
	if (export_no_arg(*env, arg, fd_out))
		return (true);
	while (arg[++j])
	{
		i = 0;
		while (arg && arg[j] && arg[j][i] != '\0' && arg[j][i] != '=')
			i++;
		new_var = ft_substr(arg[j], 0, i);
		if (!cheak_var(new_var) && free_new_var(&new_var))
			continue ;
		if (update_var(env, arg[j], new_var) && free_new_var(&new_var))
			continue ;
		new = creat_var(arg[j]);
		add_env(env, new);
		free_new_var(&new_var);
	}
	if (new_var)
		free(new_var);
	return (true);
}

bool	free_new_var(char **s)
{
	free(*s);
	*s = NULL;
	return (true);
}

bool	export_no_arg(t_env *env, char **arg, int fd_out)
{
	if (!arg || !*arg)
	{
		while (env)
		{
			ft_putstr_fd("declare -x ", fd_out);
			ft_putstr_fd(env->var, fd_out);
			if (env->value && env->value[0] == '=')
			{
				ft_putstr_fd("=", fd_out);
				ft_putstr_fd("\"", fd_out);
				ft_putstr_fd(&env->value[1], fd_out);
				ft_putstr_fd("\"", fd_out);
			}
			ft_putstr_fd("\n", fd_out);
			env = env->next;
		}
		g_status = 0;
		return (true);
	}
	return (false);
}
