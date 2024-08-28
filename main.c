/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/28 09:23:27 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_head.h"

void _lks(){system("leaks -q minishell");}

void	print_exec(t_exec *exec)
{
	t_exec *tmp = exec;
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
		if (tmp->heredoc)
			printf("[last redirection is a heredoc]\n");
		if (tmp->append)
			printf("[last redirection is a append]\n");
		while (tmp && tmp->path_option_args &&  tmp->path_option_args[i])
		{
			printf("cmd %d-> %s\n",j , tmp->path_option_args[i]);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
}

void	remove_spaces(t_elem **tokens)
{
    t_elem *temp;
    t_elem *prev = NULL;
	t_elem *current = *tokens;

	// print_tokens((*tokens));
    while (current)
    {
        if (current->type == W_SPACE && current->state == GENERAL)
        {
            temp = current;
            if (prev != NULL)
                prev->next = current->next;
            else 
                *tokens = current->next;
            if (current->next != NULL)
                current->next->prev = prev;
            current = current->next;
            free(temp);
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

void	remove_quotes(t_elem **tokens)
{
    t_elem *temp;
    t_elem *prev;
	t_elem *current = *tokens;

	// print_tokens((*tokens));
	temp = NULL;
	prev = NULL;
    while (current)
    {
        if ((current->type == D_QUOTE || current->type == S_QUOTE) && current->state == GENERAL)
        {
            temp = current;
            if (prev != NULL )
                prev->next = current->next;
            else 
                *tokens = current->next;
            if (current && current->next != NULL)
                current->next->prev = NULL;
            current = current->next;
			// free(temp);
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	
	t_env	*envi;
	char    *rl;
	t_data  *tokens;
	
	// atexit(_lks);
	signals_init();
	// envi = malloc(sizeof(t_env));
	envi = NULL;

	set_env(&envi, env); // enviroment initialize
	while (1)
	{
		init_data(&tokens);
		rl = readline(PROMPT);
		if (!rl)
			return(printf("exit\n"), 0);
		if (rl[0])
		{
			add_history(rl);
			ft_lexing(rl, &tokens);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens, envi);
				print_exec(tokens->exec);
				if (tokens && tokens->exec)
					ft_exic(tokens->exec, &envi);
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