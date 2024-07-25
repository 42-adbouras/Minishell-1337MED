/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:08:13 by eismail           #+#    #+#             */
/*   Updated: 2024/01/17 17:26:50 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*temp;
	size_t	i;
	int		size_to_allocate;
	size_t	s_len;

	i = 0;
	s_len = ft_strlen(s);
	size_to_allocate = 0;
	if (!s)
		return (NULL);
	if (start > s_len)
		return (ft_strdup(""));
	if ((ft_strlen(s + start)) > len)
		size_to_allocate = len;
	else
		size_to_allocate = ft_strlen(s + start);
	temp = malloc(sizeof(char) * (size_to_allocate + 1));
	if (temp == NULL)
		return (NULL);
	while ((start < s_len) && (s[start] != '\0') && (i < len))
		temp[i++] = s[start++];
	temp[i] = '\0';
	return (temp);
}
