/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:52:03 by eismail           #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expand_heredoc(char *temp, t_env *env, int i, char **join)
{
	int		j;
	char	*sub;
	char	*env_var;
	char	*join2;

	j = i + 1;
	while (temp[j] && temp[j] != '$')
		j++;
	sub = ft_substr(&temp[i + 1], 0, (j - i) - 1);
	j = 0;
	if (sub[j] == '?')
		env_var = get_exit_status(sub);
	else if (sub[j + 1] && sub[j + 1] != '\n')
		env_var = ft_expand(env, sub);
	else if (!sub[j + 1])
		env_var = ft_strdup("$\n");
	else
		env_var = ft_strdup("\n");
	join2 = ft_strjoin(*join, env_var);
	free(env_var);
	env_var = ft_strdup(join2);
	free (sub);
	return (free(join2), env_var);
}

char	*expand_heredoc(char **line, t_env *env, bool expand, char *delimi)
{
	char	*env_var;
	char	*temp;
	char	*join2;
	int		i;
	int		j;

	i = 0;
	j = 0;
	env_var = NULL;
	temp = ft_strjoin(*line, "\n");
	if (expand || ft_skip(&j, delimi, temp))
		return (free(*line), temp);
	while (temp[i])
	{
		join2 = ft_strdup(env_var);
		if (temp[i] == '$')
			env_var = get_expand_heredoc(temp, env, i++, &join2);
		else
			env_var = ft_substr(temp, 0, j);
		while (temp[i] && temp[i] != '$')
			i++;
	}
	free(temp);
	free(join2);
	return (free(*line), env_var);
}

void	read_heredoc(char *delimiter, int *pip, t_env *env, bool expand)
{
	char	*s;
	char	*line;

	s = ft_strjoin(delimiter, "\n");
	line = ft_strdup("");
	while (ft_strncmp(line, s, ft_strlen(s) + 1) != 0)
	{
		ft_putstr_fd(line, pip[1]);
		free(line);
		line = readline("> ");
		if (!line)
		{
			close(pip[1]);
			close(pip[0]);
			free(s);
			exit(0);
		}
		line = expand_heredoc(&line, env, expand, s);
	}
	close(pip[1]);
	close(pip[0]);
	free(line);
	free(pip);
	free(s);
	exit(0);
}

bool	wait_heredoc(int pid, int *pip, char **delimiters, int i)
{
	int		status;
	int		exit_status;

	exit_status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	if (exit_status == 1)
	{
		g_status = 300;
		close(pip[1]);
		close(pip[0]);
		return (false);
	}
	else if (exit_status == 0)
	{
		g_status = 0;
		close(pip[1]);
		return (true);
	}
	if (delimiters[i + 1])
		close(pip[0]);
	close(pip[1]);
	return (true);
}

void	if_herdoc(char **delimiters, int *fd_heredoc, t_exec *cmd, int *pip)
{
	int	i;
	int	pid;

	i = -1;
	*(fd_heredoc) = -1;
	while (delimiters && *delimiters && delimiters[++i])
	{
		if (pipe(pip) == -1)
			return (free(pip));
		pid = fork();
		if (pid == 1)
			return (free(pip));
		if (pid == 0)
		{
			signal(SIGINT, herdoc_signal);
			read_heredoc(delimiters[i], pip, cmd->env, cmd->expand_heredoc);
		}
		if (!wait_heredoc(pid, pip, delimiters, i))
			break ;
	}
	*(fd_heredoc) = pip[0];
}
