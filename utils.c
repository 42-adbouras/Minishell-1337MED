/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:19:54 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/09 12:46:12 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data **data, char *rl)
{
	*data = malloc(sizeof(t_data));
	if (!(*data))
	{
		ft_error("malloc failed!\n", 1);
		exit(1);
	}
	(*data)->tokens = NULL;
	(*data)->exec = NULL;
	ft_lexing(rl, data);
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

void	skip_quotes(t_token ***token, t_state *state)
{
	if ((**token) && ((**token)->type == D_QUOTE || (**token)->type == S_QUOTE))
		(**token) = (**token)->next;
	if (**token)
		*state = (**token)->state;
}

bool	last_heredoc(t_token *token)
{
	while (token && token->type == W_SPACE)
		token = token->next;
	if (token && token->type == WORD)
		return (true);
	return (false);
}

bool	skip_cmd(t_exec **cmd, int *arry)
{
	if (*cmd && (!arry || !(*cmd)->path_option_args[0]))
	{
		*cmd = (*cmd)->next;
		return (true);
	}
	return (false);
}
