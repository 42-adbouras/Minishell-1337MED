/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:19:54 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/02 19:22:18 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data **tokens)
{
	*tokens = malloc(sizeof(t_data));
	if (!(*tokens))
	{
		ft_error("malloc failed!\n");
		exit(1);
	}
	(*tokens)->head = NULL;
	(*tokens)->exec = NULL;
}

char	*ft_strndup(const char *s1, int n)
{
	char	*ptr;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
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
