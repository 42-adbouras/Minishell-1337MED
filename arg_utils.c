/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:14:13 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/30 12:05:12 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_cmd(t_elem *tokens, t_env *env, bool *exed)
{
	char *word;

	word = NULL;
	if (if_builtin(tokens->content))
		word = ft_strdup(tokens->content);
	if (!if_builtin(tokens->content) && !*exed)
		word = get_access(tokens->content, env);
	if (word)
		*exed = true;
	return (word);
}

int	count_words(t_elem *tokens)
{
	int count;
	t_elem *temp;

	count = 0;
	temp = tokens;
	while (temp && temp->type == W_SPACE && temp->state == GENERAL)
		temp = temp->next;
	if (!temp)
		return (0);
	while (tokens && tokens->type != PIPE )
	{
		// if (tokens && (tokens->type == D_QUOTE || tokens->type == S_QUOTE))
		// {
		// 	if(tokens->prev && tokens->prev->type == W_SPACE)
		// 		count++;
		// 	tokens = tokens->next;
		// 	while (tokens && (tokens->state ==  IN_DQUOTE || tokens->state ==  IN_SQUOTE))
		// 		tokens = tokens->next;
		// }
		// else if (tokens && tokens->type == WORD)
		// 	count++;
		if(((tokens->prev && tokens->prev->type == PIPE && tokens->prev->state == GENERAL)) || (tokens && tokens->type == W_SPACE && tokens->state == GENERAL))
		{
			count++;
			while (tokens && tokens->type == W_SPACE && tokens->state == GENERAL)
				tokens = tokens->next;
		}
		else if (!tokens->next || (tokens->next && tokens->next->type == PIPE && tokens->next->state == GENERAL))
			count++;
		if (tokens && tokens->type != PIPE)
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
            return (free_char_arr(paths), free(slash), temp);
        free(temp);
        free(slash);
        i++;
    }
    return (free_char_arr(paths), ft_strdup(cmd));
}
