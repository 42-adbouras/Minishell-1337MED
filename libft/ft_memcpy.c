/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:02:05 by eismail           #+#    #+#             */
/*   Updated: 2023/12/24 18:18:51 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*ptr;
	char		*ptr2;

	if (!dst && !src)
		return (NULL);
	ptr = (char *)dst;
	ptr2 = (char *) src;
	while (n--)
		*ptr++ = *ptr2++;
	return (dst);
}
