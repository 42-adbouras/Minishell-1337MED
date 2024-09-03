/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/03 13:10:29 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void _lks(){system("leaks -q minishell");}

int main(int ac, char **av, char **env)
{
	t_env	*envi;
	char    *rl;
	t_data  *tokens;

	(void)ac;
	(void)av;
	signals_init();
	envi = set_env(env);
	while (1)
	{
		if (ft_readline(&rl))
		{
			init_data(&tokens);
			ft_lexing(rl, &tokens);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens, envi);
				// if (tokens && tokens->exec && tokens->exec->run)
				// 	ft_exic(tokens->exec, &envi);
			}
			free_data(&tokens, &rl, 1);
		}
		else
			free_data(&tokens, &rl, 0);
	}
	clear_history();
	return (0);
}
