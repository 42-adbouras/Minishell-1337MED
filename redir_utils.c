/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:11:34 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/03 09:29:09 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_red(t_elem *tokens, t_token type)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == type && tokens->state == GENERAL)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	*get_redire(t_elem **token, t_env *env, bool *ambiguous)
{
	char	*redir_in;

	if (!token || !*token)
		return (ft_strdup(""));
	(*token) = (*token)->next;
	while ((*token) && (*token)->type == W_SPACE && (*token)->state == GENERAL)
		(*token) = (*token)->next;
	if ((*token) && ((*token)->type == S_QUOTE || (*token)->type == D_QUOTE))
		redir_in = get_arg(token, env, true);
	else if ((*token) && (*token)->type == ENV)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == WORD)
		{
			*ambiguous = true;
			redir_in = ft_expand(env, (*token)->content);
			if (!redir_in || (redir_in && redir_in[0] == '\0'))
				return (ft_error("minishell: ambiguous redirect\n"), NULL);
		}
		else
			redir_in = ft_strdup("$");
	}
	else
		redir_in = ft_strdup((*token)->content);
	return (redir_in);
}

char	*get_heredoc(t_elem **token, bool *heredoc)
{
	char	*redir;
	char	*temp;

	(*token) = (*token)->next;
	while ((*token) && (*token)->type == W_SPACE)
		(*token) = (*token)->next;
	if (((*token)->type == S_QUOTE
			|| (*token)->type == D_QUOTE) && (*token)->next)
	{
		*heredoc = true;
		redir = get_arg(token, NULL, true);
	}
	else if ((*token)->type == ENV)
	{
		(*token) = (*token)->next;
		temp = ft_strndup((*token)->content, (*token)->len);
		redir = ft_strjoin("$", temp);
		free(temp);
	}
	else
		redir = ft_strndup((*token)->content, (*token)->len);
	return (redir);
}

bool	last_heredoc(t_elem *token)
{
	while (token && token->type == W_SPACE)
		token = token->next;
	if (token && token->type == WORD)
		return (true);
	return (false);
}
