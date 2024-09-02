/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:20:15 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/02 10:56:59 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void if_redir(t_elem **token)
{
	(*token) = (*token)->next;
		
	while ((*token) && (*token)->type == W_SPACE && (*token)->state == GENERAL)
		(*token) = (*token)->next;
	if ((*token) && (*token)->type == ENV && (*token)->state == GENERAL)
	{
		(*token) = (*token)->next;
		if (*token)
			(*token) = (*token)->next;
	}
	while ((*token) && (((*token)->type != W_SPACE && (*token)->type != PIPE) || (((*token)->type == W_SPACE && (*token)->type == PIPE) && (*token)->state == GENERAL)))
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
		if (temp->type == WORD && !new->exed && temp->next && (temp->next->type != S_QUOTE  && temp->next->type != D_QUOTE))
			new->path_option_args[i++] = get_cmd(temp, env, &new->exed);
		else if (temp && ((temp->type == S_QUOTE  || temp->type == D_QUOTE) || (temp->type == WORD && temp->state == GENERAL)))
			new->path_option_args[i++] = get_arg(&temp, env, new->exed);
		else if (temp && temp->type == ENV )
			new->path_option_args[i++] =  process_expander(&temp, env);
		else if (temp && is_red(temp->type) && temp->state == GENERAL)
			if_redir(&temp);
		if (temp && temp->type != PIPE)
			temp = temp->next;
	}
	new->path_option_args[i] = NULL;
	if (!process_redir(tokens, &new, env))
		return (free_exec(&new), NULL); // leaks fixed
	return (new);
}

void    new_exec_node(t_exec **new, t_elem *tokens, t_env *env)
{
	int n;
	int out;
	
	n = count_words(tokens);
	out = (count_red(tokens, REDIR_OUT) + count_red(tokens, REDIR_APP));
	(*new) = malloc(sizeof(t_exec));
	if (!n)
		(*new)->run = false;
	else
		(*new)->run = true;
	(*new)->path_option_args = malloc(sizeof(char *) * (n + 1));
	(*new)->redir_in = malloc(sizeof(char *) * (count_red(tokens, REDIR_IN) + 1));
	(*new)->redir_out = malloc(sizeof(char *) * (out + 1));
	(*new)->heredoc_end = malloc(sizeof(char *) * (count_red(tokens, REDIR_AND) + 1));
	(*new)->path_option_args[n] = NULL;
	(*new)->redir_in[count_red(tokens, REDIR_IN)] = NULL; 
	(*new)->redir_out[out] = NULL; 
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
