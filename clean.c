/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:08:10 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/31 16:05:16 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (arr && arr[i])
	{
		if (arr[i])
			free(arr[i++]);
	}
	free(arr);
}
