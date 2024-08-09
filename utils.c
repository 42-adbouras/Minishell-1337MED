/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:19:54 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/09 12:21:30 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data **tokens)
{
	*tokens = malloc(sizeof(t_data));
	if (!(*tokens))
		printf("malloc!\n");
	(*tokens)->head = NULL;
	// (*tokens)->tail = NULL;
	(*tokens)->size = 0;
}

char	*ft_strndup(const char *s1, int n)
{
	char	*ptr;
	int		len;
	int		i;

	len = ft_strlen(s1);
	ptr = malloc(len + 1);
	i = 0;
	if (!ptr)
		return (NULL);
	while (i < len && i < n)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
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
