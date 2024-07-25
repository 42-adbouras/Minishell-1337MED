/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:41:02 by eismail           #+#    #+#             */
/*   Updated: 2024/02/09 11:48:55 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*temp;
	size_t	t_size;

	if (size != 0 && count > SIZE_MAX / size)
		return (0);
	t_size = size * count;
	temp = malloc(t_size);
	if (temp != NULL)
		ft_bzero(temp, t_size);
	return (temp);
}
