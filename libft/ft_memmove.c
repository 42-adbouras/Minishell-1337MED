/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:18:03 by eismail           #+#    #+#             */
/*   Updated: 2023/12/24 18:27:13 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*ptr1;
	unsigned char		*ptr2;
	size_t				i;

	ptr1 = (unsigned char *)dst;
	ptr2 = (unsigned char *) src;
	if (src < dst)
	{
		i = len;
		while (i--)
			ptr1[i] = ptr2 [i];
	}
	else
		ft_memcpy(ptr1, ptr2, len);
	return (dst);
}
