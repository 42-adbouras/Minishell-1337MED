/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/15 13:20:14 by adbouras         ###   ########.fr       */
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

// void	get_red(t_exec **new, t_elem *token)
// {
// 	t_elem	*tmp;

// 	tmp = skip_wspace(token->next, 'N');
	
// }
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
		// else if (is_red(temp->type))
		// {
		// 	get_red(&new, temp);
		// }
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
		while (tmp->path_option_args[i])
		{
			printf("%d-> %s\n",j , tmp->path_option_args[i++]);
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