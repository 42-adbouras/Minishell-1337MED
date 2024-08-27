/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:14:13 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/27 09:49:53 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_cmd(t_elem *tokens, t_env *env, bool *exed)
{
	char *word;

	word = ft_strdup(tokens->content);
	if (!word)
		return (NULL);
	if (!if_builtin(word) && !*exed)
		word = get_access(word, env);
	if (word)
		*exed = true;
	return (word);
}
char	*get_spichil(t_elem **temp, char **new, t_env *env)
{
	if ((*temp) && ((*temp)->content[ft_strlen((*temp)->content) - 1] == '=') && *new)
	{
		while ((*temp) && (*temp)->type != S_QUOTE && (*temp)->type != D_QUOTE)
			(*temp) = (*temp)->next;
		*new = ft_strjoin(*new, get_arg(temp, env));
	}
	else if ((*temp) && (*temp)->content[0] == '-' && *new && (*temp)->next)
	{
		(*temp) = (*temp)->next;
		if ((*temp) && ((*temp)->type == S_QUOTE || (*temp)->type == D_QUOTE))
			*new = ft_strjoin(*new, get_arg(temp, env));
	}
	return (*new);
}
int	count_words(t_elem *tokens)
{
	int count;

	count = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens && (tokens->state == IN_DQUOTE || tokens->state == IN_SQUOTE))
		{
			count++;
			tokens = tokens->next;
			while (tokens && (tokens->state ==  IN_DQUOTE || tokens->state ==  IN_SQUOTE))
				tokens = tokens->next;
		}
		else if (tokens && tokens->type == WORD)
			count++;
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

char	*get_access(char *cmd, t_env *env)
{
	char    *temp;
    char    *slash;
    char    **paths;
    int        i;

    i = 0;
	while (env && ft_strncmp(env->var, "PATH", 5))
		env = env->next;
    if (!access(cmd, X_OK) || !env)
        return (cmd);
	paths = ft_split(env->value, ':');
    while (paths[i])
    {
        slash = ft_strjoin("/", cmd);
        temp = ft_strjoin(paths[i], slash);
        if (!access(temp, X_OK))
            return (free(slash), temp);
        free(temp);
        free(slash);
        i++;
    }
    return (cmd);
}
