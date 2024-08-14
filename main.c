/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/14 19:58:31 by adbouras         ###   ########.fr       */
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

char	*get_arg(t_elem *token)
{
	t_elem	*tmp;
	char	*arg;
	t_state	state;

	arg = NULL;
	tmp = token->next;
	state = token->next->state;
	while (tmp && tmp->state == state)
	{
		arg = ft_strjoin(arg, tmp->content);
		tmp = tmp->next;
	}
	return (arg);
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
			new->path_option_args[i++] = get_arg(temp);
		}
		temp = temp->next;
	}
	new->path_option_args[i] = NULL;
	return (new);
}

void	exec_add_back(t_exec *exec, t_exec *new)
{
	t_exec	*last;

	if (!exec && !new)
		return ;
	if (!exec)
	{
		exec = new;
		return ;
	}
	last = exec;
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
		new = new_exec((*data)->head);
		exec_add_back((*data)->exec, new);
		while (temp && temp->type != PIPE)
			temp = temp->next;
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