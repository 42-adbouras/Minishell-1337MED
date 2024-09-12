/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:14:13 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 12:46:12 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd(t_token *tokens, t_env *env, bool *exed)
{
	char	*word;

	word = NULL;
	if (if_builtin(tokens->content))
		word = ft_strdup(tokens->content);
	if (!if_builtin(tokens->content) && !*exed)
		word = get_access(tokens->content, env);
	if (word)
		*exed = true;
	return (word);
}

bool	skip_space(t_token **tokens, int *count)
{
	while ((*tokens) && (*tokens)->type == W_SPACE
		&& (*tokens)->state == GENERAL)
		(*tokens) = (*tokens)->next;
	if (!(*tokens))
		return (false);
	(*count)++;
	return (true);
}

int	count_words(t_token *tokens)
{
	int		count;
	t_token	*temp;

	count = 0;
	temp = tokens;
	while (temp && temp->type == W_SPACE && temp->state == GENERAL)
		temp = temp->next;
	while (not_pipe(tokens))
	{
		if (should_skip(tokens))
		{
			if (!skip_space(&tokens, &count))
				break ;
		}
		else if (is_word(tokens))
			count++;
		if (increment(tokens))
			tokens = tokens->next;
	}
	if (!temp || count < 0)
		return (0);
	return (count);
}

char	*get_executeble(char *s)
{
	char	*temp;

	if (!ft_strncmp(s, "./", 2))
	{
		temp = ft_substr(s, 2, ft_strlen(s));
		if (!access(temp, X_OK))
			return (free(s), temp);
		else
			return (free(temp), ft_strdup(s));
	}
	else
		return (ft_strdup(s));
}

char	*get_access(char *cmd, t_env *env)
{
	char	*temp;
	char	*slash;
	char	**paths;
	int		i;

	i = 0;
	while (env && ft_strncmp(env->var, "PATH", 5))
		env = env->next;
	if (!ft_strncmp(cmd, "../", 3) || !ft_strncmp(cmd, "/", 1)
		|| !ft_strncmp(cmd, "./", 2) || cmd[0] == '\0'
		|| !env)
		return (get_executeble(cmd));
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
