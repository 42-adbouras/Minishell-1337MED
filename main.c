/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:12 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/25 17:55:57 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ensure the next output starts on a new line >> rl_on_new_line();
bool	ft_pros_arg(t_cmd *data)
{
	int		i;
	char	**paths;
	char	*p;

	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	data->split = ft_split(data->args, ' ');
	if (!access(data->split[0], X_OK))
		return (true);
	while (paths[i])
	{
		p = ft_strjoin(ft_strjoin(paths[i], "/"), data->split[0]);
		if (!access(p, X_OK))
		{
			data->path = p;
			return (true);
		}
		free(p);
		i++;
	}
	i = 0;
	// while (paths[i])
	// 	free(paths[i++]);
	return (false);
}

// void	ft_init(t_cmd *data)
// {
// 	// data->path = getenv("PATH");
// 	// data->args = "";
// }

int	main()
{
	t_cmd	*data = NULL;

	data = malloc(sizeof(t_cmd));
	// ft_init(data);
	while (1)
	{
		data->args = readline("minishell-$ ");
		if (!data->args)
			break;
		if(!ft_pros_arg(data))
			printf("minishell: command not found: %s\n", data->args);
		free(data->args);
		printf("-%s-\n", data->path);
	}
	free (data);
	// system("leaks -q minishell");
	return (0);
}
