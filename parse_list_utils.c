/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:20:15 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/22 17:04:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*new_exec(t_elem *tokens, t_env *env)
{
	t_exec	*new;
	t_elem	*temp;
	int	i;

	i = 0;
	temp = tokens;
	new_exec_node(&new, tokens);
	while (temp && temp->type != PIPE)
	{
		
		if (temp->type == WORD)
			new->path_option_args[i++] = get_cmd(temp);
		else if (temp->type == S_QUOTE && temp->next)
			new->path_option_args[i++] = get_arg(&temp, env);
		else if (temp->type == D_QUOTE && temp->next)
			new->path_option_args[i++] = get_arg(&temp, env);
		else if (temp->type == ENV)
			process_expander(temp, &new, env, &i);
		temp = temp->next;
	}
	process_redir(tokens, &new);
	return (new);
}

void    new_exec_node(t_exec **new, t_elem *tokens)
{
	(*new) = malloc(sizeof(t_exec));
	(*new)->path_option_args = malloc(sizeof(char *) * (count_words(tokens) + 1));
	(*new)->redir_in = malloc(sizeof(char *) * (count_red(tokens, REDIR_IN) + 1));
	(*new)->redir_out = malloc(sizeof(char *) * (count_red(tokens, REDIR_OUT) + 1));
	(*new)->heredoc_end = malloc(sizeof(char *) * (count_red(tokens, REDIR_AND) + 1));
	(*new)->path_option_args[count_words(tokens)] = NULL; 
	(*new)->redir_in[count_red(tokens, REDIR_IN)] = NULL; 
	(*new)->redir_out[count_red(tokens, REDIR_OUT)] = NULL; 
	(*new)->heredoc_end[count_red(tokens, REDIR_AND)] = NULL;
	(*new)->append = false;
	(*new)->heredoc = false;
	(*new)->next = NULL;
}

void	exec_add_back(t_exec **exec, t_exec *new)
{
	t_exec	*last;

	if (!*exec && !new)
		return ;
	if (!*exec)
	{
		*exec = new;
		return ;
	}
	last = *exec;
	while (last->next)
	{
		last = last->next;
	}
	last->next = new;
}

void	init_exec_struct(t_data **data, t_env *env)
{
	t_exec	*new;
	t_elem	*temp;
	
	temp = (*data)->head;
	while (temp)
	{
		new = new_exec(temp, env);
		exec_add_back(&(*data)->exec, new);
		while (temp && temp->type != PIPE)
			temp = temp->next;
		 if (temp)
            temp = temp->next;
	}
}
