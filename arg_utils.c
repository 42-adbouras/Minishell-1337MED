/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:14:13 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 14:49:38 by adbouras         ###   ########.fr       */
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
bool skip_space(t_elem **tokens, int *count)
{
	while ((*tokens) && (*tokens)->type == W_SPACE && (*tokens)->state == GENERAL)
		(*tokens) = (*tokens)->next;
	if (!(*tokens))
		return (false);
	(*count)++;
	return (true);
}

int	count_words(t_elem *tokens)
{
	int count;
	t_elem *temp;

	count = 0;
	temp = tokens;
	while (temp && temp->type == W_SPACE && temp->state == GENERAL)
		temp = temp->next;
	while ((tokens && tokens->type != PIPE ) || (tokens &&(tokens)->type == PIPE && (tokens)->state != GENERAL))
	{
		if(((tokens->prev && tokens->prev->type == PIPE && tokens->prev->state == GENERAL)) || (tokens && tokens->type == W_SPACE && tokens->state == GENERAL))
		{
			if (!skip_space(&tokens, &count))
				break ;
		}
		else if (!tokens->next || (tokens->next && (tokens->next->type == W_SPACE || tokens->next->type == PIPE) && tokens->next->state == GENERAL))
			count++;
		// if (tokens && is_red(tokens->type) && tokens->state == GENERAL)
		// 	skip_redir(&tokens);
		if ((tokens && tokens->type != PIPE) || (tokens && (tokens)->type == PIPE && (tokens)->state != GENERAL))
			tokens = tokens->next;
	}
	if (!temp || count < 0)
		return (0);
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
    if (cmd[0] == '\0' || !access(cmd, X_OK) || !env)
        return (ft_strdup(cmd)); // pointer being freed was not allocated ./test.sh fixed
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
