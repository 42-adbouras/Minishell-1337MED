/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:20:15 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/28 21:49:41 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void if_redir(t_elem **token)
{
	(*token) = (*token)->next;
	while ((*token) && ((*token)->type == W_SPACE || (*token)->type == ENV))
		(*token) = (*token)->next;
}

t_exec	*new_exec(t_elem *tokens, t_env *env)
{
	t_exec	*new;
	t_elem	*temp;
	int	i;

	i = 0;
	temp = tokens;
	new_exec_node(&new, tokens, env);
	while (temp && temp->type != PIPE)
	{
		if (temp->type == WORD && !new->exed)
		{
			if (ft_strncmp(&temp->content[ft_strlen(temp->content) - 3], ".sh", 4) == 0)
				new->path_option_args[i++] = ft_strdup("/bin/bash");
			new->path_option_args[i++] = get_cmd(temp, env, &new->exed);
		}
		else if (temp->type == WORD && temp->state == GENERAL)
			new->path_option_args[i++] = ft_strdup(temp->content);
		else if (temp && (temp->type == S_QUOTE  || temp->type == D_QUOTE))
			new->path_option_args[i++] = get_arg(&temp, env, new->exed);
		else if (temp && temp->type == ENV )
			process_expander(&temp, &new, env, &i);
		else if (temp && is_red(temp->type))
			if_redir(&temp);
		if (temp)
			temp = temp->next;
	}
	if (!process_redir(tokens, &new, env))
		return (NULL);
	new->path_option_args[i] = NULL;
	return (new);
}

void    new_exec_node(t_exec **new, t_elem *tokens, t_env *env)
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
	(*new)->exed = false;
	(*new)->append = false;
	(*new)->heredoc = false;
	(*new)->env = env;
	(*new)->next = NULL;
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
	// last = *exec;
	// while (last->next)
	// {
	// 	last = last->next;
	// }
	last = ft_last_exec(*exec);
	last->next = new;
}

void join_tokens(t_elem **tokens)
{
    t_elem	*current;
    t_elem	*temp;
	char	*new;

    current = *tokens;
    while (current && current->next)
    {
        if (current->type == WORD && current->next->type == WORD)
        {
            new = ft_strjoin(current->content, current->next->content);
            free(current->content);
            current->content = new;
            current->len = ft_strlen(current->content);
			current->state = GENERAL;
            temp = current->next;
            current->next = current->next->next;
            if (current->next)
                current->next->prev = current;
            free(temp);
        }
        else
            current = current->next;
    }
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
		while (temp && (temp->type != PIPE || (temp->type == PIPE && temp->state != GENERAL)))
			temp = temp->next;
		if (temp)
            temp = temp->next;
	}
}
