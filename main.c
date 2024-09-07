/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/07 15:50:46 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_env	*envi;
	char	*rl;
	t_data	*tokens;

	(void)ac;
	(void)av;
	signals_init();
	envi = set_env(env);
	while (1)
	{
		if (ft_readline(&rl))
		{
			init_data(&tokens, rl);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens, envi);
				if (tokens && tokens->exec && tokens->exec->run)
					ft_exic(tokens->exec, &envi);
			}
			free_data(&tokens, &rl, 1);
		}
		else
			free_data(&tokens, &rl, 0);
	}
	return (clear_history(), 0);
}
