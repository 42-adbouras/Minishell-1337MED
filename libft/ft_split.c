/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:08:04 by eismail           #+#    #+#             */
/*   Updated: 2024/05/23 10:10:24 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	check_free(char *ptr, char **split_str, size_t nb)
{
	int	i;

	i = 0;
	if (!ptr)
	{
		split_str[nb] = NULL;
		while (split_str[i])
		{
			free(split_str[i]);
			i++;
		}
		free(split_str);
	}
}

static	size_t	count_strings(const char *s, char c)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			count++;
			while ((s[i] != c) && (s[i] != '\0'))
				i++;
		}
	}
	return (count);
}

static	char	**fillout(char const *str, char **ptr, char c, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	nb;
	char	*temp;

	i = 0;
	nb = 0;
	while (str[i] && nb < n)
	{
		while (str[i] == c)
			i++;
		j = i;
		while ((str[i] != c) && (str[i] != '\0'))
			i++;
		temp = ft_substr(str, j, i - j);
		check_free(temp, ptr, nb);
		if (!temp)
			return (NULL);
		ptr[nb] = temp;
		nb++;
		if (str[i] != '\0')
			i++;
	}
	ptr[nb] = NULL;
	return (ptr); 
}

char	**ft_split(char const *s, char c)
{
	char	**split_str;
	size_t	nbr_string;

	nbr_string = 0;
	if (s == NULL || s[0] == '\0')
		return (NULL);
	nbr_string = count_strings(s, c);
	split_str = (char **)malloc((nbr_string + 1) * sizeof (char *));
	if (split_str == NULL)
		return (NULL);
	split_str = fillout(s, split_str, c, nbr_string);
	if (!split_str)
		return (NULL);
	return (split_str);
}
