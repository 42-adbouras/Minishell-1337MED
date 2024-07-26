/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:12 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/26 09:46:42 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ensure the next output starts on a new line >> rl_on_new_line();

int	main()
{
	t_data	*data;
	
	data->path = getenv("PATH");
	printf("%s\n", data->path);
	while (1)
	{
		data->rl = readline("minishell$ ");
		printf("%s\n", data->rl);
	}
	// access();
	system("leaks -q minishell");
	return (0);
}
