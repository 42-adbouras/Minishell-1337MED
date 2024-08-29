/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/29 16:43:13 by adbouras         ###   ########.fr       */
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

char	*get_prompt()
{
	char	*s;
	char	*prompt;
	char	*temp;
	int		i;
	int		j;
	
	s = malloc(sizeof(char) * MAX_PATH);
	getcwd(s, MAX_PATH);
	i = ft_strlen(s);
	j = i;
	while (j > 0)
	{
		if (s[j] == '/')
			break ;
		j--;
	}
	temp = ft_substr(s, j + 1, i - j);
	prompt = ft_strjoin(temp, " ~ ");
	free (temp);
	free (s); 
	return (prompt);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	
	t_env	*envi;
	char    *rl;
	t_data  *tokens;
	char	*prompt;
	
	// atexit(_lks);
	signals_init();
	// envi = malloc(sizeof(t_env));
	envi = NULL;
	set_env(&envi, env); // enviroment initialize
	while (1)
	{
		init_data(&tokens);
		prompt = get_prompt();
		rl = readline(prompt);
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
			free (prompt);
		}
		else
		{
			free (prompt);
			free(rl);
			free(tokens);
		}
		system ("leaks -q minishell");
	}
	free_env(&envi);
	clear_history();
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'