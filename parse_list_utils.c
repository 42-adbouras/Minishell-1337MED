/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:20:15 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/09 12:46:12 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_exec_struct(t_data **data, t_env *env)
{
	t_exec	*new;
	t_token	*temp;

	temp = (*data)->tokens;
	while (temp)
	{
		new = new_exec(temp, env);
		if (!new)
		{
			ft_error("malloc failed!\n", 1);
			free_data(data, NULL, 1);
			exit(1);
		}
		exec_add_back(&(*data)->exec, new);
		while (temp && (temp->type != PIPE
				|| (temp->type == PIPE && temp->state != GENERAL)))
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
}

t_exec	*new_exec(t_token *tokens, t_env *env)
{
	t_exec	*new;
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	new_exec_node(&new, tokens);
	init_exec_node(&new, tokens, env);
	while (temp && temp->type != PIPE)
	{
		if (cmd_getter(temp, new))
			new->path_option_args[i++] = get_cmd(temp, env, &new->exed);
		else if (arg_getter(temp))
			new->path_option_args[i++] = get_arg(&temp, env, new->exed);
		else if (temp && temp->type == ENV)
			new->path_option_args[i++] = ft_expander(&temp, env, new->exed);
		else if (temp && is_red(temp->type) && temp->state == GENERAL)
			if_redir(&temp);
		if (temp && temp->type != PIPE)
			temp = temp->next;
	}
	new->path_option_args[i] = NULL;
	process_redir(tokens, &new, env);
	return (new);
}

void	new_exec_node(t_exec **new, t_token *tokens)
{
	int	n;
	int	out;

	n = count_words(tokens);
	out = (count_red(tokens, REDIR_OUT) + count_red(tokens, REDIR_APP));
	(*new) = malloc(sizeof(t_exec));
	if (!(*new))
		return ;
	if (!n)
		(*new)->run = false;
	else
		(*new)->run = true;
	(*new)->path_option_args = malloc(sizeof(char *) * (n + 1));
	(*new)->redir_in = malloc(sizeof(char *)
			* (count_red(tokens, REDIR_IN) + 1));
	(*new)->redir_out = malloc(sizeof(char *) * (out + 1));
	(*new)->heredoc_end = malloc(sizeof(char *)
			* (count_red(tokens, REDIR_AND) + 1));
}

t_exec	*ft_last_exec(t_exec *exec)
{
	if (exec == NULL)
		return (NULL);
	while (exec->next != NULL)
		exec = exec->next;
	return (exec); 
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
	last = ft_last_exec(*exec);
	last->next = new;
}
