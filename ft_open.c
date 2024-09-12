/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:06:52 by eismail           #+#    #+#             */
/*   Updated: 2024/09/09 11:54:53 by adbouras         ###   ########.fr       */
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
	if (!fds && heredoc > 0)
		return (close(heredoc), NULL);
	else if (!fds)
		return (NULL);
	if (cmd->heredoc)
	{
		if (fds[0] > 0)
			close(fds[0]);
		fds[0] = heredoc;
	}
	else if (!cmd->heredoc && heredoc > 0)
		close(heredoc);
	return (fds);
}

int	*open_redir(t_exec *cmd)
{
	int	*fds;

	fds = malloc(sizeof(int) * 2);
	fds[1] = -1;
	fds[0] = -1;
	if (!open_redir_in(cmd, fds))
		return (free(fds), NULL);
	if (!open_redir_out(cmd, fds))
		return (free(fds), NULL);
	return (fds);
}

bool	open_redir_in(t_exec *cmd, int *fds)
{
	int	i;

	i = -1;
	while (cmd && cmd->redir_in && cmd->redir_in[++i])
	{
		if (check_ambiguous(cmd->redir_in[i], cmd->ambiguous))
			return (false);
		fds[0] = open(cmd->redir_in[i], O_RDONLY);
		if (fds[0] == -1)
		{
			g_status = 1;
			perror("minishell");
			return (false);
		}
		if (cmd->redir_in[i + 1])
			close (fds[0]);
	}
	if (cmd->ambiguous && check_ambiguous(cmd->redir_in[i], cmd->ambiguous))
		return (false);
	return (true);
}

bool	open_redir_out(t_exec *cmd, int *fds)
{
	int	i;

	i = -1;
	while (cmd && cmd->redir_out && cmd->redir_out[++i])
	{
		if (check_ambiguous(cmd->redir_out[i], cmd->ambiguous))
			return (false);
		if (cmd->append)
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fds[1] == -1)
		{
			g_status = 1;
			perror("minishell");
			return (false);
		}
		if (cmd->redir_out[i + 1])
			close (fds[1]);
	}
	if (cmd->ambiguous && check_ambiguous(cmd->redir_out[i], cmd->ambiguous))
		return (false);
	return (true);
}

bool	check_ambiguous(char *file, bool error)
{
	char	**split;

	split = ft_split(file, ' ');
	if (!split)
	{
		g_status = 1;
		if (error)
			ft_putstr_fd("minishell2: ambiguous redirect\n", 2);
		return (true);
	}
	if (split && split[0] && split[1])
	{
		g_status = 1;
		if (error)
			ft_putstr_fd("minishell3: ambiguous redirect\n", 2);
		return (free_char_arr(split), true);
	}
	else
		return (free_char_arr(split), false);
}
