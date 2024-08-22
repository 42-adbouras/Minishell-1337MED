/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:11:34 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/22 17:13:24 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_red(t_elem *tokens, t_token type)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == type)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	*get_redir(t_elem *token)
{
	char	*redir;
	t_elem	*temp;
	
	temp = token;
	while (temp && temp->type != WORD)
		temp = temp->next;
	redir = ft_strndup(temp->content, temp->len);
	if (!redir)
		return (NULL);
	return (redir);
}

char *get_redirec(t_elem **token)
{
	char *redir_in;

	(*token) = (*token)->next;
	while ((*token) && (*token)->type == W_SPACE)
		(*token) = (*token)->next;
	redir_in = ft_strdup((*token)->content);
	return (redir_in);
}

bool last_heredoc(t_elem *token)
{
	while (token && token->type == W_SPACE)
		token = token->next;
	if (token && token->type == WORD)
		return (true);
	return (false);
}
