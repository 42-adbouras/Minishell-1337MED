/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:25:37 by eismail           #+#    #+#             */
/*   Updated: 2023/12/20 17:32:15 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	if (!*needle)
		return ((char *)haystack);
	while ((i < len) && (haystack[i] != '\0'))
	{
		j = 0;
		while ((i + j < len) && (needle[j] != '\0')
			&& (haystack[i + j] == needle[j]))
		{
			if (needle[j + 1] == '\0')
				return ((char *)haystack + i);
			j++;
		}
		i++;
	}
	return (NULL);
}
