/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 12:01:27 by eismail           #+#    #+#             */
/*   Updated: 2023/12/23 19:23:13 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ptr;

	i = ft_strlen(s1);
	ptr = malloc(sizeof(char) * (i + 1));
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, s1, i + 1);
	return (ptr);
}
