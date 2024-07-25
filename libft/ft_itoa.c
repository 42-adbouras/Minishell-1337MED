/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:09:06 by eismail           #+#    #+#             */
/*   Updated: 2023/12/23 22:43:49 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	fillout(char *s, int num, int i)
{
	long	nbr;

	if (num < 0) 
		nbr = -(long)num;
	else
		nbr = num;
	while (i > 0)
	{
		i--;
		s[i] = ((nbr % 10) + '0');
		nbr /= 10;
	}
	if (num < 0)
		s[0] = '-';
}

char	*ft_itoa(int n)
{
	int		size_to_allocate;
	long	nbr;
	char	*str;

	size_to_allocate = 0;
	nbr = n;
	if (n == 0)
		size_to_allocate = 1;
	if (nbr < 0)
	{
		size_to_allocate++;
		nbr *= -1;
	}
	while (nbr != 0)
	{
		nbr /= 10;
		size_to_allocate++;
	}
	str = malloc((size_to_allocate + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[size_to_allocate] = '\0';
	fillout(str, n, size_to_allocate);
	return (str);
}
