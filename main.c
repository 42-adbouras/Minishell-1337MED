/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/02 12:47:35 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_head.h"

void _lks(){system("leaks -q minishell");}

int main(int ac, char **av, char **env)
{
	t_env	*envi;
	char    *rl;
	t_data  *tokens;

	// atexit(_lks);
	(void)ac;
	(void)av;
	signals_init();
	envi = NULL;
	set_env(&envi, env); // enviroment initialize
	while (1)
	{
		if (ft_readline(&rl))
		{
			init_data(&tokens);
			ft_lexing(rl, &tokens);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens, envi);
				// print_exec(tokens->exec);
				// print_tokens(tokens);
				if (tokens && tokens->exec && tokens->exec->run)
					ft_exic(tokens->exec, &envi);
			}
			free_data(&tokens, &rl, 1);
		}
		else
			free_data(&tokens, &rl, 0);
		system ("leaks -q minishell");
	}
	clear_history();
	return (0);
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'