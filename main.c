/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/09 11:21:56 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_data **tokens)
{
	t_elem *node = (*tokens)->head;
	t_elem *temp;
	
	node = (*tokens)->head;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	free(*tokens);
}


int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char    *rl;
	t_data  *tokens;

	while (1)
	{
		init_data(&tokens);
		rl = readline(PROMPT);
		if (!rl)
			break ;
		if (rl[0])
		{
			add_history(rl);
			ft_lexing(rl, &tokens);
			if (if_syntax_err(tokens))
				continue;
			free (rl);
			free_tokens(&tokens);	
		}
		// system ("leaks minishell");
	}
	clear_history();
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'