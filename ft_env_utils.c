/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:28:05 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 10:48:14 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_last(t_env *env)
{
	if (env == NULL)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env); 
}


void free_env(t_env **env)
{
    t_env *current;
    t_env *temp;

	current = *env;
    while (current)
    {
		temp = current;
		current = current->next;
        free(temp->var);
        free(temp->value);
        free(temp);
    }
}

char **env_to_str(t_env *env)
{
	t_env *temp;
	char **envstr;
	int i;
	
	i = 0;
	temp = env;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	envstr = malloc (sizeof(char *) * (i + 1));
	i = 0;
	while (env)
	{
		envstr[i] = ft_strjoin(env->var, env->value);
		env = env->next;
		i++;
	}
	envstr[i] = NULL;
	return (envstr);
}
