/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:02:17 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 15:02:58 by adbouras         ###   ########.fr       */
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
