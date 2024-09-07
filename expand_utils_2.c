/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:02:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/07 15:55:57 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *var)
{
	char	*variable;
	int		i;

	i = 0;
	while (var[i] && (var[i] == '_' || ft_isalnum(var[i])))
		i++;
	variable = ft_substr(var, 0, i);
	return (variable);
}

char	*arg_join(t_elem *token, char **arg, char *join)
{
	char	*join2;

	join2 = NULL;
	if (token && (token->type == D_QUOTE
			|| token->type == S_QUOTE) && token->state == GENERAL)
		join2 = ft_strjoin(*arg, join);
	else
		join2 = ft_strjoin(*arg, (token)->content);
	free(*arg);
	return (join2);
}

char	*get_after(char *var)
{
	char	*after;
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	after = NULL;
	if (var[0] && !ft_isalpha(var[0]) && var[0] != '_')
	{
		after = ft_substr(var, 1, ft_strlen(var));
		return (after);
	}
	while (var[i] && (ft_isalnum(var[i]) || var[i] == '_'))
	{
		i++;
	}
	if (i > 0)
		after = ft_substr(var, i, ft_strlen(var));
	return (after);
}
