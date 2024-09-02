/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 18:14:23 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/02 08:31:30 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *state_to_string(t_state state) {
    switch (state)
	{
        case IN_DQUOTE: return "IN_DQUOTE";
        case IN_SQUOTE: return "IN_SQUOTE";
        case GENERAL: return "GENERAL";
    }
}

char	*token_to_string(t_token token)
{
	switch (token)
	{
		case WORD: return "WORD";
		case W_SPACE: return "W_SPACE";
		case D_QUOTE: return "D_QUOTE";
		case S_QUOTE: return "S_QUOTE";
		case PIPE: return "PIPE";
		case REDIR_IN: return "REDIR_IN";
		case REDIR_AND: return "REDIR_AND";
		case REDIR_OUT: return "REDIR_OUT";
		case REDIR_APP: return "REDIR_APP";
		case ENV: return "ENV";
	}
}

void    print_tokens(t_data *tokens)
{
    t_elem	*tmp;

    tmp = tokens->head;
    while (tmp)
	{
		printf("\'%s\'\t\t%d\t\t%s\t\t%s\n", tmp->content, tmp->len, token_to_string(tmp->type), state_to_string(tmp->state));
		tmp = tmp->next;
	}
}

void	print_exec(t_exec *exec)
{
	t_exec	*tmp = exec;
	int		i = 0;
	int		j = 1;
	while (tmp)
	{
		i = 0;
		for(int x = 0; tmp->redir_in[x]; x++)
			printf("redir_in %d-> %s\n",j , tmp->redir_in[x]);
		for(int y = 0; tmp->redir_out[y]; y++)
			printf("redir_out %d-> %s\n",j , tmp->redir_out[y]);
		for(int z = 0; tmp->heredoc_end[z]; z++)
			printf("heredoc_end %d-> %s\n",j , tmp->heredoc_end[z]);
		if (tmp->heredoc)
			printf("[last redirection is a heredoc]\n");
		if (tmp->append)
			printf("[last redirection is a append]\n");
		while (tmp && tmp->path_option_args &&  tmp->path_option_args[i])
		{
			printf("cmd %d-> %s\n",j , tmp->path_option_args[i]);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
}