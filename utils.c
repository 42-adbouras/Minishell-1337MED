/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:19:54 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/03 17:31:39 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data **tokens, char *rl)
{
	*tokens = malloc(sizeof(t_data));
	if (!(*tokens))
	{
		ft_error("malloc failed!\n", 1);
		exit(1);
	}
	(*tokens)->head = NULL;
	(*tokens)->exec = NULL;
	ft_lexing(rl, tokens);
}

char	*ft_strndup(const char *s1, int n)
{
	char	*ptr;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	ptr = malloc(len + 1);
	i = 0;
	if (!ptr)
		return (NULL);
	while (i < len && i < n)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	skip_quotes(t_elem ***token, t_state *state)
{
	if ((**token) && ((**token)->type == D_QUOTE || (**token)->type == S_QUOTE))
		(**token) = (**token)->next;
	if (**token)
		*state = (**token)->state;
}
