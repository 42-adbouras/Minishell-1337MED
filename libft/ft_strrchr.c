/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 17:18:40 by eismail           #+#    #+#             */
/*   Updated: 2023/12/20 17:32:45 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = 0;
	if ((char)c == '\0')
		return ((char *)s + ft_strlen(s));
	while (s[i])
	{
		if ((char)s[i] == (char)c)
			ptr = (char *)(s + i);
		i++;
	}
	return (ptr);
}
