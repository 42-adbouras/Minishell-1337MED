/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:08:10 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/01 21:07:54 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_data(t_data **data, char **rl, int flag)
{
	if (*rl)
	{
		free(*rl);
		*rl = NULL;	
	}
	if (*data)
    {
        if (flag == 1)
        {
            free_tokens(&(*data)->head);
            free_exec(&(*data)->exec);
        }
        free(*data);
        *data = NULL;
    }
}

void	free_tokens(t_elem **tokens)
{
	t_elem *node;
	t_elem *temp;
	
	node = (*tokens);
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
}

void	free_exec(t_exec **exec)
{
	t_exec	*node;
	t_exec	*temp;

	node = *exec;
	while (node)
	{
		temp = node;
		node = node->next;
		if (temp->path_option_args)
			free_char_arr(temp->path_option_args);
		if (temp->redir_in)
			free_char_arr(temp->redir_in);
		if (temp->redir_out)
			free_char_arr(temp->redir_out);
		if (temp->heredoc_end)
			free_char_arr(temp->heredoc_end);
		free(temp);
	}
}

void	free_char_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (arr[i])
			free(arr[i++]);
	}
	free(arr);
}
