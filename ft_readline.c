/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 19:46:07 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_readline(char **rl)
{
	*rl = readline("minishell> ");
	if (!*rl)
	{
		clear_history();
		printf("exit\n");
		exit (g_status);
	}
	if (!*rl[0])
		return (0);
	add_history(*rl);
	return (1);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*temp;

	temp = ft_getcwd();
	prompt = ft_strjoin(temp, ": ");
	free (temp);
	return (prompt);
}

char	*ft_getcwd(void)
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
	return (temp);
}
