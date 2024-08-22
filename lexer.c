/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:11:03 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/22 12:26:36 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lexing(char *read, t_data **tokens)
{
	t_state	state;
	int		i;

	i = 0;
	state = GENERAL;
	while (read[i])
	{
		i = ft_tokenize(*tokens, read, i, &state);
	}
	print_tokens(*tokens);
}

int	ft_tokenize(t_data *tokens, char *read, int i, t_state *status)
{
	if (!is_grammar(read[i]))
		i += get_word(tokens, &read[i], *status);
	else if (is_white_space(read[i]))
		token_add_back(tokens, new_token(&read[i++], 1, W_SPACE, *status));
	else if (read[i] == '|')
		token_add_back(tokens, new_token(&read[i++], 1, PIPE, *status));
	else if (read[i] == '>' || read[i] == '<')
		i += red_token(tokens, read, i, status);
	else if (read[i] == '\"')
		quote_token(tokens, &read[i++], D_QUOTE, status);
	else if (read[i] == '\'')
		quote_token(tokens, &read[i++], S_QUOTE, status);
	else if (read[i] == '$')
		token_add_back(tokens, new_token(&read[i++], 1, ENV, *status));
	return (i);
}

int	get_word(t_data *tokens, char *read, t_state status)
{
	char	*sub;
	int		i;

	i = 0;
	while (read[i] && !is_grammar(read[i]))
		i++;
	sub = ft_substr(read, 0, i);
	token_add_back(tokens, new_token(sub, i, WORD, status));
	free (sub);
	return (i);
}

int	red_token(t_data *tokens, char *read, int i, t_state *status)
{
	int	old_i;

	old_i = i;
	if (read[i] == '>')
	{
		if (read[i + 1] == '>')
			token_add_back(tokens, new_token(&read[i++],
					2, REDIR_APP, *status));
		else
			token_add_back(tokens, new_token(&read[i],
					1, REDIR_OUT, *status));
		i++;
	}
	else if (read[i] == '<')
	{
		if (read[i + 1] == '<')
			token_add_back(tokens, new_token(&read[i++],
					2, REDIR_AND, *status));
		else
			token_add_back(tokens, new_token(&read[i],
					1, REDIR_IN, *status));
		i++;
	}
	return (i - old_i);
}

void	quote_token(t_data *tokens, char *read, t_token type, t_state *status)
{
	t_state	new_state;
	t_token	new_type;

	if (type == D_QUOTE)
	{
		new_state = IN_DQUOTE;
		new_type = D_QUOTE;
	}
	else
	{
		new_state = IN_SQUOTE;
		new_type = S_QUOTE;
	}
	if (*status == GENERAL)
	{
		token_add_back(tokens, new_token(read, 1, new_type, GENERAL));
		*status = new_state;
	}
	else if (*status == new_state)
	{
		*status = GENERAL;
		token_add_back(tokens, new_token(read, 1, new_type, *status));
	}
	else
		token_add_back(tokens, new_token(read, 1, new_type, *status));
}
