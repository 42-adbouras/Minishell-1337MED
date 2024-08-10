/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/10 17:37:36 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void _lks(){system("leaks -q minishell");}

t_exec	*init_exec(t_elem *tokens)
{
	
}

void	init_exec_struct(t_data **data)
{
	init_exec((*data)->head);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char    *rl;
	t_data  *tokens;
	
	atexit(_lks);
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
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens);
				continue;
				
			}
			free (rl);
			free_tokens(&tokens);	
		}
		// system ("leaks minishell");
	}
	clear_history();
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'