/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/23 10:16:15 by adbouras         ###   ########.fr       */
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
		while (tmp->path_option_args[i])
		{
			printf("cmd %d-> %s\n",j , tmp->path_option_args[i]);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
}
void	sig_handler(int sig, siginfo_t *info, void *context)
{
	(void) info;
	(void) context;

	if (sig == SIGINT)
		return;
	if (sig == SIGQUIT)
		return;
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	
	t_sa	sa;
	t_env	*envi;
	char    *rl;
	t_data  *tokens;
	
	// atexit(_lks);

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &sig_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);

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