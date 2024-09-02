/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 19:46:07 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 08:29:24 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_readline(char **rl)
{
	char	*prompt;

	prompt = get_prompt();
	*rl = readline(prompt);
	if (!*rl)
	{
		free(prompt);
		clear_history();
		printf("exit\n");
		exit (0);
	}
	if (!*rl[0])
		return (free(prompt), 0);
	add_history(*rl);
	free(prompt);
	return (1);
}

char	*get_prompt()
{
	char	*prompt;
	char	*temp;

	temp = ft_getcwd();
	prompt = ft_strjoin(temp, ": ");
	free (temp);
	return (prompt);
}

char	*ft_getcwd()
{
	char	*s;
	char	*temp;
	int		i;
	int		j;
	
	s = malloc(sizeof(char) * MAX_PATH);
	getcwd(s, MAX_PATH);
	i = ft_strlen(s);
	j = i;
	while (j > 0)
	{
		if (s[j] == '/')
			break ;
		j--;
	}
	temp = ft_substr(s, j + 1, i - j);
	free(s);
	return(temp);
}
