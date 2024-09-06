/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:11:34 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/06 17:53:04 by eismail          ###   ########.fr       */
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
		redir_in = get_arg(token, env, true);
		if (check_ambiguous(redir_in, false))
			*ambiguous = true;
		if (*ambiguous)
			return (NULL);
	}
	else
		redir_in = get_arg(token, env, true);
	if (!redir_in)
		redir_in = ft_strdup("");
	return (redir_in);
}

char *get_delim(t_elem **token)
{
	char	*arg;
	char	*join;
	t_state	state;

	arg = NULL;
	join = ft_strdup("");
	skip_quotes(&token, &state);
	while ((*token) && ((*token)->state == state))
	{
		arg = arg_join(*token, &arg, join);
		rest_function(token, &state);
		if ((*token) && (((*token)->type == W_SPACE
					|| (*token)->type == PIPE) && (*token)->state == GENERAL))
			break ;
	}
	return (check_exec(true, &arg, &join, NULL));
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
		redir = get_delim(token);
	}
	else if ((*token) && (*token)->type == ENV )
	{
		(*token) = (*token)->next;
		if ((*token))
			temp = ft_strndup((*token)->content, (*token)->len);
		else 
			temp = ft_strdup("");
		redir = ft_strjoin("$", temp);
		free(temp);
	}
	else
		redir = get_arg(token, NULL, true);
	// redir = ft_strndup((*token)->content, (*token)->len);
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
