/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:08:10 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/28 13:00:57 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_data **tokens)
{
	t_elem *node = (*tokens)->head;
	t_elem *temp;
	
	node = (*tokens)->head;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	free_exec(&(*tokens)->exec);
	free(*tokens);
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
		free_char_arr(temp->path_option_args);
		free_char_arr(temp->redir_in);
		free_char_arr(temp->redir_out);
		free_char_arr(temp->heredoc_end);
	}
	free(*exec);
}

void	delete_token(t_elem **token)
{
	t_elem	*next;
	t_elem	*prev;
	t_elem	*temp;

	temp = (*token);	
	next = (*token)->next;
	prev = (*token)->prev;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	free (temp->content);
	free (temp);
}

void	free_char_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
