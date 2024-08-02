/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:12 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/01 10:55:50 by adbouras         ###   ########.fr       */
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


void	node_init(t_cmd **node)
{
	(*node) = malloc(sizeof(t_cmd));
	(*node)->path = NULL;
	(*node)->rl = NULL;
	(*node)->cmd = NULL;
	(*node)->option = NULL;
	(*node)->split = NULL;
	(*node)->red_in = NULL;
	(*node)->red_out = NULL;
}

bool	is_white_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' ||
		c == '\r' || c == '\v' || c == '\f')
		return (true);
	return (false);
}

char	*remove_spaces(char *str)
{
	char	*s;
	int		i;
	int		j;

	s = malloc(ft_strlen(str));
	i = 0;
	j = 0;
	while (str[i] && is_white_space(str[i]))
		i++;
	while (str[i])
	{
		if (is_white_space(str[i]) && str[i + 1])
		{
			while (str[i] && is_white_space(str[i]))
				i++;
			s[j++] = ' ';
		}
		s[j++] = str[i++];
	}
	s[j] = '\0';
	free (str);
	return (s);
}

char	**ft_get_tokens(char *cmd)
{
	char	*sub;
	char	*sub2;
	char	**res;
	int		i;
	int		j;
	
	i = 0;
	while (cmd[i] && (cmd[i] != '\'' && cmd[i] != '\"'))
		i++;
	sub = ft_substr(cmd, 0, i-1);
	sub2 = ft_substr(cmd, i, ft_strlen(cmd));
	res = ft_split(sub, ' ');
	j = 0;
	while (res[j])
		j++;
	res[j] = ft_strjoin(res[j], sub2);
	free(sub);
	return (res);
}

void	if_executable(t_cmd *data)
{
	t_cmd	*temp;
	char	**tokens;
	int i = 0;
	temp = data;
	while (temp)
	{
		tokens = ft_get_tokens(data->cmd);
		while (tokens[i])
			printf("%s\n", tokens[i++]);
		temp = temp->pipe;
	}
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
	pipe = ft_split((*data)->rl, '|');
	while (pipe[i])
	{
		pipe[i] = remove_spaces(pipe[i]);
		new = new_node(pipe[i]);
		node_add_back(data, new);
		i++;
	}
	if_executable(*data);
		// return (false);
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

	data = NULL;
	while (1)
	{
		node_init(&data);
		data->rl = readline("minishell-$ ");
		if (!data->rl)
		{
			printf("exit\n");
			break;
		}
		if(!ft_pros_arg(&data))
			printf("minishell: command not found: %s\n", data->rl);
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
