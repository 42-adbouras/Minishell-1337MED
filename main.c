/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/09 11:54:26 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_env	*envi;
	char	*rl;
	t_data	*data;

	(void)ac;
	(void)av;
	signals_init();
	envi = set_env(env);
	while (1)
	{
		if (ft_readline(&rl) && rl[0])
		{
			init_data(&data, rl);
			if (!if_syntax_err(data))
			{
				init_exec_struct(&data, envi);
				if (data && data->exec && data->exec->run)
					ft_exic(data->exec, &envi);
			}
			free_data(&data, &rl, 1);
		}
		else
			free(rl);
	}
	return (clear_history(), 0);
}
