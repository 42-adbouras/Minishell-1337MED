/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:16:37 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 17:16:55 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_cd(char *path, t_env *env)
{
	t_env	*home_env;

	home_env = env;
	while (home_env && ft_strncmp(home_env->var, "HOME", 5))
		home_env = home_env->next;
	if (path == NULL)
	{
		path = &home_env->value[1];
		chdir(path);
	}
	if (chdir(path) != 0)
	{
		if (errno != ENOENT)
			fprintf(stderr, "minishell: cd : Permission denied\n");
		else
			perror("minishell: cd ");
		g_status = 1;
		return (true);
	}
	g_status = 0;
	return (true);
}
