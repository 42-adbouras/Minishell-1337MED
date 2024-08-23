/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:28:05 by eismail           #+#    #+#             */
/*   Updated: 2024/08/21 19:30:44 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_head.h"

t_env	*ft_last(t_env *env)
{
	if (env == NULL)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env); 
}

void add_env(t_env **head, t_env *env_new)
{
	t_env *last;
	
	if (*head == NULL)
		*head = env_new;
	else 
	{
		last = ft_last(*head);
		last->next = env_new;
	}
}

t_env *creat_var(char *var)
{
	int j;
	t_env *envi;

	envi = malloc(sizeof(t_env));
	j = -1;
	while(var[++j] != '=')
	;
	envi->var = ft_substr(var, 0,j);
	envi->value = ft_substr(var, ++j,ft_strlen(var));
	envi->next = NULL;
	return (envi);
}

void set_env(t_env **envi, char **env)
{
	int i;
	t_env *new;

	i = -1;
	while (env[++i])
	{
		new = creat_var(env[i]);
		add_env(envi, new);
	}
}

void free_env(t_env *env)
{
    t_env *temp;

    temp = env;
    while (temp)
    {
        free(temp->var);
        free(temp->value);
        free(temp);
        temp = env->next;
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
		envstr[i] = ft_strjoin(env->var, "=");
		envstr[i] = ft_strjoin(envstr[i], env->value);
		env = env->next;
		i++;
	}
	envstr[i] = NULL;
	return (envstr);
}