/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:11:34 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 12:46:12 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_red(t_token *tokens, t_type type)
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

char	*get_redire(t_token **token, t_env *env, bool *ambiguous)
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
		if (check_ambiguous(redir_in, false)
			|| (redir_in && redir_in[0] == '\0'))
			*ambiguous = true;
		if (*ambiguous)
			return (free(redir_in), NULL);
	}
	else
		redir_in = get_arg(token, env, true);
	if (!redir_in)
		redir_in = ft_strdup("");
	return (redir_in);
}

char	*get_delim(t_token **token)
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

char	*get_heredoc(t_token **token, bool *heredoc)
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
	else if ((*token) && (*token)->type == ENV)
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
	return (redir);
}
