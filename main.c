/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:46:31 by adhambouras       #+#    #+#             */
/*   Updated: 2024/08/22 10:40:58 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_head.h"

void _lks(){system("leaks -q minishell");}

char *get_cmd(t_elem *tokens)
{
	char *word;

	word = ft_strdup(tokens->content);
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

char *ft_expand(t_env *env, char *var)
{
	while(env)
	{
		if (!ft_strncmp(env->var, var, ft_strlen(env->var) + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
char	*get_arg(t_elem **token, t_env *env)
{
	char	*arg;
	t_state	state;

	arg = NULL;
	(*token) = (*token)->next;
	state = (*token)->state;
	while ((*token) && (*token)->state == state)
	{
		if ((*token) && (*token)->type == ENV && (*token)->state == IN_DQUOTE)
		{
			(*token) = (*token)->next;
			if ((*token) && (*token)->type == WORD)
				arg = ft_strjoin(arg, ft_expand(env, (*token)->content));
			else
				arg = ft_strjoin(arg,ft_strdup("$"));
		}
		else
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

char *get_redirec(t_elem **token)
{
	char *redir_in;

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
char	*get_access(char *cmd, char **env)
{
	char	*temp;
	char	*slash;
	int		i;

	i = 0;
	if (!access(cmd, X_OK))
		return (cmd);
	while (env[i])
	{
		slash = ft_strjoin(cmd, "/");
		temp = ft_strjoin(env[i], slash);
		if (!access(temp, X_OK))
			return (free(slash), temp);
		free(temp);
		free(slash);
		i++;
	}
	return (NULL);
}

void	process_redir(t_elem *tokens, t_exec **new)
{
	t_elem	*temp;
	
	temp = tokens;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIR_IN )
		{
			(*new)->heredoc = false;
			*((*new)->redir_in)++ = get_redirec(&temp);
		}
		else if (temp->type == REDIR_OUT || temp->type == REDIR_APP)
		{
			(*new)->append = false;
			if (temp->type == REDIR_APP)
				(*new)->append = true;
			*((*new)->redir_out)++ = get_redirec(&temp);
		}
		else if (temp->type == REDIR_AND)
		{
			*((*new)->heredoc_end)++ = get_redirec(&temp);
			(*new)->heredoc = last_heredoc(temp);
		}
		temp = temp->next;
	}
}

void	process_expander(t_elem *temp, t_exec **new, t_env *env, int *i)
{
	if (temp->next)
	{
		temp = temp->next;
		if (temp->type == WORD)
			(*new)->path_option_args[(*i)++] = ft_expand(env, temp->content);
		if (temp->content[ft_strlen(temp->content) - 1] == '=')
        {
            while (temp && temp->type != S_QUOTE && temp->type != D_QUOTE)
                temp = temp->next;
            (*new)->path_option_args[*i - 1] = ft_strjoin((*new)->path_option_args[*i - 1], get_arg(&temp, env));
        }
		else
			(*new)->path_option_args[(*i)++] = ft_strdup("$");
	}
	else
		(*new)->path_option_args[(*i)++] = ft_strdup("$");
}

t_exec	*new_exec(t_elem *tokens, t_env *env)
{
	t_exec	*new;
	t_elem	*temp;
	int	i;

	i = 0;
	temp = tokens;
	new_exec_node(&new, tokens);
	while (temp && temp->type != PIPE)
	{
		
		if (temp->type == WORD)
			new->path_option_args[i++] = get_cmd(temp);
		else if (temp->type == S_QUOTE && temp->next)
			new->path_option_args[i++] = get_arg(&temp, env);
		else if (temp->type == D_QUOTE && temp->next)
			new->path_option_args[i++] = get_arg(&temp, env);
		else if (temp->type == ENV)
			process_expander(temp, &new, env, &i);
		temp = temp->next;
	}
	process_redir(tokens, &new);
	return (new);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	// (void)env;
	t_env *envi;
	
	char    *rl;
	t_data  *tokens;
	
	// atexit(_lks);
	set_env(&envi, env); // enviroment initialize
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
				init_exec_struct(&tokens, envi);
				t_exec *tmp = (tokens)->exec;
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