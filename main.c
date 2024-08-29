/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/29 16:54:38 by eismail          ###   ########.fr       */
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
    // t_elem *temp;
    // t_elem *prev;
	t_elem *current = *tokens;

	// print_tokens((*tokens));
	// temp = NULL;
	// prev = NULL;
    while (current)
    {
        if ((current->type == D_QUOTE || current->type == S_QUOTE) && current->state == GENERAL)
        {
			if (current->next && (current->next->type == D_QUOTE || current->next->type == S_QUOTE) && current->state == GENERAL)
			{
				free(current->content);
				delete_token(&current->next);
				current->content = ft_strdup("");
				current->type = WORD;
				// current = current->next;
			}
			else
				delete_token(&current);
		}
		current = current->next;
    }
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	
	t_env	*envi;
	char    *rl;
	t_data  *tokens;
	
	// atexit(_lks);
	signals_init();
	// envi = malloc(sizeof(t_env));
	envi = NULL;

	set_env(&envi, env); // enviroment initialize
	// char **strenv;
	// strenv = env_to_str(envi);
	// free_env(&envi);
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
				// print_exec(tokens->exec);
				// print_tokens(tokens);
				if (tokens && tokens->exec)
					ft_exic(tokens->exec, &envi);
			}
			free(rl);
			free_tokens(&tokens->head);
			free_exec(&tokens->exec);
			free(tokens);
		}
		else
		{
			free(rl);
			free(tokens);
		}
		// free_char_arr(strenv);
		// system ("leaks -q minishell");
	}
	
	clear_history();
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'