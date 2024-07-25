/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:33:38 by eismail           #+#    #+#             */
/*   Updated: 2023/12/23 17:41:28 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	n;

	i = 0;
	if (!s1)
		return (NULL);
	n = ft_strlen(s1);
	if (!set)
		return (ft_strdup(s1));
	while ((s1[i] != '\0') && (ft_strchr(set, s1[i])))
		i++;
	while ((n > i) && (ft_strchr(set, s1[n - 1])))
		n--;
	return (ft_substr(s1, i, n - i));
}
