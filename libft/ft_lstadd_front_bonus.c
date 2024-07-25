/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:53:09 by eismail           #+#    #+#             */
/*   Updated: 2023/12/28 16:32:24 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!*lst && new)
	{
		*lst = new;
		return ;
	}
	if (new)
	{
		new->next = *lst;
		*lst = new;
	}
}
