/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:12 by adbouras          #+#    #+#             */
/*   Updated: 2024/07/31 09:30:33 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ensure the next output starts on a new line >> rl_on_new_line();

void	ft_add_slash(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		i++;
	}
}


// void	node_init(char **pipes)
// {
// 	t_cmd	*new;
// 	int		i;

// 	i = 0;
	
// }

char	*remove_spaces(char *str)
{
	char	*s;
	int		i;
	int		j;

	s = malloc(ft_strlen(str));
	i = 0;
	j = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		if (str[i] == ' ' && str[i + 1])
		{
			while (str[i] && str[i] == ' ')
				i++;
			s[j++] = ' ';
		}
		s[j++] = str[i++];
	}
	s[j] = '\0';
	free (str);
	return (s);
}

bool	ft_pros_arg(t_cmd **data)
{
	int		i;
	char	**paths;
	char	**pipe;
	t_cmd	*new;
	
	// t_cmd	*tmp;
	// char	*p;

	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	pipe = ft_split((*data)->args, '|');
	while (pipe[i])
	{
		pipe[i] = remove_spaces(pipe[i]);
		new = new_node(pipe[i]);
		node_add_back(data, new);
		i++;
	}
	// tmp = data;
	// while (tmp)
	// {
	// 	printf("|%s|\n", tmp->cmd);
	// 	tmp = tmp->pipe;
	// }
	// ft_add_slash(paths);
	// while (paths[i])
	// {
	// 	p = ft_strjoin(paths[i], data->split[0]);
		// if (!access(p, X_OK))
		// {
		// 	data->path = p;
		// 	return (true);
		// }
	// 	free(p);
	// 	i++;
	// }
	return (true);
}


int	main()
{
	t_cmd	*data;
	t_cmd	*tmp;

	// ft_init(data);
	while (1)
	{
		data = malloc(sizeof(t_cmd));
		data->args = readline("minishell-$ ");
		if (!data->args)
		{
			printf("exit\n");	
			break;
		}
		if(!ft_pros_arg(&data))
			printf("minishell: command not found: %s\n", data->args);
		tmp = data;
		while (tmp)
		{
			printf("|%s|\n", tmp->cmd);
			tmp = tmp->pipe;
		}
		clear_nodes(&data);
		free (data);
	}
	// system("leaks -q minishell");
	return (0);
}
