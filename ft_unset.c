/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:30:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 17:38:33 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_unset(t_env **env, char **toDelete)
{
	t_env	*temp;
	t_env	*prev;
	int		i;

	i = -1;
	while (toDelete[++i])
	{
		if (!cheak_var(toDelete[i]))
			return (true);
		temp = *env;
		prev = NULL;
		while (temp)
		{
			if (!ft_strncmp(toDelete[i], temp->var, ft_strlen(temp->var) + 1))
			{
				remove_var(&prev, &temp, &env);
				return (true);
			}
			prev = temp;
			temp = temp->next;
		}
	}
	return (true);
}

void	remove_var(t_env **prev, t_env **var, t_env ***env)
{
	if (!*prev)
		**env = (*var)->next;
	else
		(*prev)->next = (*var)->next;
	free((*var)->var);
	free((*var)->value);
	free(*var);
	*var = NULL;
}
