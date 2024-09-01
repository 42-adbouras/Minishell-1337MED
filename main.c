/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/09/01 21:17:34 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_head.h"

void _lks(){system("leaks -q minishell");}

void	print_exec(t_exec *exec)
{
	t_exec	*tmp = exec;
	int		i = 0;
	int		j = 1;
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

int main(int ac, char **av, char **env)
{
	t_env	*envi;
	char    *rl;
	t_data  *tokens;

	atexit(_lks);
	(void)ac;
	(void)av;
	signals_init();
	envi = NULL;
	set_env(&envi, env); // enviroment initialize
	while (1)
	{
		if (ft_readline(&rl))
		{
			init_data(&tokens);
			ft_lexing(rl, &tokens);
			if (!if_syntax_err(tokens))
			{
				init_exec_struct(&tokens, envi);
				// print_exec(tokens->exec);
				// print_tokens(tokens);
				if (tokens && tokens->exec && tokens->exec->run)
					ft_exic(tokens->exec, &envi);
			}
			free_data(&tokens, &rl, 1);
		}
		else
			free_data(&tokens, &rl, 0);
		system ("leaks -q minishell");
	}
	clear_history();
	return (0);
}
// echo "hello $USER " > file | grep h | cat << eof | cat >> file | echo 'done'