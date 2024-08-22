/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:14:13 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/22 17:14:38 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_cmd(t_elem *tokens)
{
	char *word;

	word = ft_strdup(tokens->content);
	if (!word)
		return (NULL);
	return (word);
}

int	count_words(t_elem *tokens)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	*get_access(char *cmd, char **env)
{
	char	*temp;
	char	*slash;
	int		i;

	i = 0;
	if (!access(cmd, X_OK))
		return (cmd);
	while (env[i])
	{
		slash = ft_strjoin(cmd, "/");
		temp = ft_strjoin(env[i], slash);
		if (!access(temp, X_OK))
			return (free(slash), temp);
		free(temp);
		free(slash);
		i++;
	}
	return (NULL);
}
