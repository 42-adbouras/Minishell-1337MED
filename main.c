/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/18 19:09:40 by eismail          ###   ########.fr       */
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

int	count_red(t_elem *tokens, t_token type)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == type)
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
	state = (*token)->state;
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

// bool	get_access(char *path)
// {
// 	if (access)
// }

char *get_redirec(t_elem **token)
{
	t_elem *temp;
	char *redir_in;
	int i;
	
	temp = *token;
	i = 0;
	(*token) = (*token)->next;
	while ((*token) && (*token)->type == W_SPACE)
		(*token) = (*token)->next;
	redir_in = ft_strdup((*token)->content);
	return (redir_in);
}
bool last_heredoc(t_elem	*token)
{
	while (token && token->type == W_SPACE)
		token = token->next;
	if (token && token->type == WORD)
		return (true);
	return (false);
}
t_exec	*new_exec(t_elem *tokens)
{
	t_exec	*new;
	t_elem	*temp;
	int	i;
	int	j;
	int	l;
	int	n;

	i = 0;
	j = 0;
	l = 0;
	n = 0;
	temp = tokens;
	new = malloc(sizeof(t_exec));
	new->path_option_args = malloc(sizeof(char *) * (count_words(tokens) + 1));
	new->redir_in = malloc(sizeof(char *) * (count_red(temp, REDIR_IN) + 1));
	new->redir_out = malloc(sizeof(char *) * (count_red(temp, REDIR_OUT) + 1));
	new->heredoc_end = malloc(sizeof(char *) * (count_red(temp, REDIR_AND) + 1));
	new->append = false;
	new->heredoc = false;
	new->next = NULL;
	while (temp && temp->type != PIPE)
	{
		
		if (temp->type == WORD)
		{
			new->path_option_args[i] = get_cmd(temp);
			if (!new->path_option_args)
				return (NULL);
			// if (!get_access(new->path_option_args[i]))
			// 	return (NULL);
			i++;
		}
		else if ((temp->type == D_QUOTE || temp->type == S_QUOTE) && temp->next)
		{
			new->path_option_args[i++] = get_arg(&temp);
		}
		else if (temp->type == ENV  && temp->next)
		{
			temp = temp->next;
			ft_expand();
			new->path_option_args[i++] = get_arg(&temp);
		}
		else if (is_red(temp->type))
		{
			if (temp->type == REDIR_IN )
			{
				new->redir_in[i] = get_redirec(&temp);
				j++;
			}
			if (temp->type == REDIR_OUT || temp->type == REDIR_APP)
			{
				new->append = false;
				if (temp->type == REDIR_APP)
					new->append = true;
				new->redir_out[l] = get_redirec(&temp);
				l++;
			}
			if (temp->type == REDIR_AND)
			{
				new->heredoc_end[n] = get_redirec(&temp);
				new->heredoc = last_heredoc(temp);
				n++;
			}
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
		// for(int x = 0; tmp->redir_in[x]; x++)
		// 	printf("redir_in %d-> %s\n",j , tmp->redir_in[x]);
		// for(int y = 0; tmp->redir_out[y]; y++)
		// 	printf("redir_out %d-> %s\n",j , tmp->redir_out[y]);
		// for(int z = 0; tmp->heredoc_end[z]; z++)
		// 	printf("heredoc_end %d-> %s\n",j , tmp->heredoc_end[z]);
		// if (tmp->heredoc)
		// 	printf("[last redirection is a heredoc]\n");
		// if (tmp->append)
		// 	printf("[last redirection is a append]\n");
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