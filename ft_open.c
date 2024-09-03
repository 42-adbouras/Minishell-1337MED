/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:06:52 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 17:00:59 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*ft_open(t_exec *cmd)
{
	int	heredoc;
	int	*fds;
	int	*pip;

	heredoc = 0;
	pip = malloc(sizeof(int) * 2);
	if (!pip)
		return (NULL);
	if (cmd->heredoc_end)
		if_herdoc(cmd->heredoc_end, &heredoc, cmd, pip);
	free(pip);
	fds = open_redir(cmd);
	if (!fds)
		return (NULL);
	if (cmd->heredoc)
		fds[0] = heredoc;
	return (fds);
}

int	*open_redir(t_exec *cmd)
{
	int	*fds;

	fds = malloc(sizeof(int) * 2);
	fds[1] = -1;
	fds[0] = -1;
	if (!open_redir_in(cmd, fds))
		return (NULL);
	if (!open_redir_out(cmd, fds))
		return (NULL);
	return (fds);
}

bool	open_redir_in(t_exec *cmd, int *fds)
{
	int	i;

	i = -1;
	while (cmd && cmd->redir_in && cmd->redir_in[++i])
	{
		if (cmd->ambiguous)
		{
			if (check_ambiguous(cmd->redir_in[i]))
				return (free(fds), false);
		}
		fds[0] = open(cmd->redir_in[i], O_RDONLY);
		if (fds[0] == -1)
		{
			g_status = 1;
			perror("minishell");
			return (free(fds), false);
		}
		if (cmd->redir_in[i + 1])
			close (fds[0]);
	}
	return (true);
}

bool	open_redir_out(t_exec *cmd, int *fds)
{
	int	i;

	i = -1;
	while (cmd && cmd->redir_out && cmd->redir_out[++i])
	{
		if (cmd->ambiguous)
		{
			if (check_ambiguous(cmd->redir_out[i]))
				return (free(fds), false);
		}
		if (cmd->append)
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fds[1] == -1)
		{
			g_status = 1;
			perror("minishell");
			return (free(fds), false);
		}
		if (cmd->redir_out[i + 1])
			close (fds[1]);
	}
	return (true);
}

bool	check_ambiguous(char *file)
{
	char	**split;

	split = ft_split(file, ' ');
	if (!split)
	{
		g_status = 1;
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (true);
	}
	if (split && split[0] && split[1])
	{
		g_status = 1;
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (free_char_arr(split), true);
	}
	else
		return (free_char_arr(split), false);
}
