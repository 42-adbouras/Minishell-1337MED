/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/15 20:13:07 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_head.h"

void _lks(){system("leaks -q minishell");}

char *get_cmd(t_elem *tokens)
{
	char *word;

	word = ft_strndup(tokens->content, tokens->len);
	if (!word)
		return (NULL);
	return (word);
}

int	count_words(t_elem *tokens)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	count_red(t_elem *tokens)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (is_red(tokens->type))
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	*get_arg(t_elem **token)
{
	char	*arg;
	t_state	state;

	arg = NULL;
	(*token) = (*token)->next;
	state = (*token)->next->state;
	while ((*token) && (*token)->state == state)
	{
		arg = ft_strjoin(arg, (*token)->content);
		(*token) = (*token)->next;
	}
	return (arg);
}

char	*get_redir(t_elem *token)
{
	char	*redir;
	t_elem	*temp;
	
	temp = token;
	while (temp && temp->type != WORD)
		temp = temp->next;
	redir = ft_strndup(temp->content, temp->len);
	if (!redir)
		return (NULL);
	return (redir);
}
void	get_red(t_exec **new, t_elem **token)
{
	t_token	type;
	t_token	temp;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	type = (*token)->type;
	while (*token && (*token)->type != PIPE)
	{
		if (type == REDIR_IN)
		{
			(*new)->redir_in[i++] = get_redir(*token);
			temp = (*token)->type;	
		}
		else if (type == REDIR_OUT)
		{
			(*new)->redir_out[j++] = get_redir(*token);
			temp = (*token)->type;	
		}
		else if (type == REDIR_APP)
		{
			(*new)->redir_out[j++] = get_redir(*token);
			temp = (*token)->type;
		}
		else if (type == REDIR_AND)
		{
			(*new)->heredoc_end[k++] = get_redir(*token);
			temp = (*token)->type;
		}
		(*token) = (*token)->next;
	}
	(*new)->redir_in[i] = NULL;
	(*new)->redir_out[j] = NULL;
	(*new)->heredoc_end[k] = NULL;
	if (temp == REDIR_APP)
		(*new)->append = true;
	else if (temp == REDIR_AND)
		(*new)->heredoc = true;
}
t_exec	*new_exec(t_elem *tokens)
{
	t_exec	*new;
	t_elem	*temp;
	int	i;

	i = 0;
	temp = tokens;
	new = malloc(sizeof(t_exec));
	new->path_option_args = malloc(sizeof(char *) * (count_words(tokens) + 1));
	new->redir_in = malloc(sizeof(char *) * (count_red(tokens) + 1));
	new->redir_out = malloc(sizeof(char *) * (count_red(tokens) + 1));
	new->heredoc_end = malloc(sizeof(char *) * (count_red(tokens) + 1));
	new->append = false;
	new->heredoc = false;
	new->next = NULL;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == WORD)
		{
			new->path_option_args[i++] = get_cmd(temp);
			if (!new->path_option_args)
				return (NULL);
		}
		else if ((temp->type == D_QUOTE || temp->type == S_QUOTE) && temp->next)
		{
			new->path_option_args[i++] = get_arg(&temp);
		}
		else if (is_red(temp->type))
		{
			get_red(&new, &temp);
		}
		temp = temp->next;
	}
	new->path_option_args[i] = NULL;
	return (new);
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

void	init_exec_struct(t_data **data)
{
	t_exec	*new;
	t_elem	*temp;
	
	temp = (*data)->head;
	while (temp)
	{
		new = new_exec(temp);
		exec_add_back(&(*data)->exec, new);
		while (temp && temp->type != PIPE)
			temp = temp->next;
		 if (temp)
            temp = temp->next;
	}
	t_exec *tmp = (*data)->exec;
	int i = 0;
	int j = 1;
	while (tmp)
	{
		i = 0;
		for(int x = 0; tmp->redir_in[x]; x++)
			printf("redir_in %d-> %s\n",j , tmp->redir_in[x]);
		for(int y = 0; tmp->redir_out[y]; y++)
			printf("redir_out %d-> %s\n",j , tmp->redir_out[y]);
		for(int z = 0; tmp->heredoc_end[z]; z++)
			printf("heredoc_end %d-> %s\n",j , tmp->heredoc_end[z]);
		while (tmp->path_option_args[i])
		{
			printf("cmd %d-> %s\n",j , tmp->path_option_args[i]);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	// t_env *envi;
	
	char    *rl;
	t_data  *tokens;
	
	// atexit(_lks);
	// set_env(&envi, env); // enviroment initialize
	while (1)
	{
		init_data(&tokens);
		rl = readline(PROMPT);
		if (!rl)
			break ;
		if (rl[0])
		{
			add_history(rl);
			ft_lexing(rl, &tokens);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens);
				continue;
				
			}
			free (rl);
			free_tokens(&tokens);
			// free_env(envi); // free enviroment struct
		}
		// system ("leaks minishell");
	}
	clear_history();
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'