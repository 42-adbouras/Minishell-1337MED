/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:02:41 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 10:04:56 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>
#include <fcntl.h>

int ft_count_cmd(t_exec *cmd)
{
	int i;
	t_exec *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}


void	free_int(int **p, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(p[i]);
		i++;
	}
	free(p);
}

int	**ft_pip(int cmd_num)
{
	int	i;
	int	**fd;

	i = 0;
	fd = malloc(sizeof(int *) * cmd_num);
	if (!fd)
		return (NULL);
	while (i < cmd_num)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i++])
		{
			free_int(fd, i);
			return (NULL);
		}
	}
	i = 0;
	while ((i < cmd_num - 1))
	{
		if (pipe(fd[i]) == -1)
			return (free_int(fd, cmd_num), NULL);
		i++;
	}
	return (fd);
}

void	ft_close(int cmd_num, int **pipes, int *fds)
{
	int	i;

	i = 0;
	while (i < cmd_num - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	if (fds && fds[0] != -1)
		close(fds[0]);
	if (fds && fds[1] != -1)
		close(fds[1]);
}

void	ft_stdin(int **pipes, int pid, int *fds)
{
	if (fds && fds[0] != -1)
	{
		dup2(fds[0], STDIN_FILENO);
	}
	else if (pid != 0)
	{
		dup2(pipes[pid - 1][0], STDIN_FILENO);
	}
}

void	ft_stdout(int cmd_num, int **pipes, int pid, int *fds)
{
	if (fds && fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
	}
	else if (pid != cmd_num - 1)
	{
		dup2(pipes[pid][1], STDOUT_FILENO);
	}
}

bool fd_hindler(int cmd_num, int **fd, int  *fds, int pos)
{	
	ft_stdin(fd, pos, fds);
	ft_stdout(cmd_num, fd, pos, fds);
	ft_close(cmd_num, fd, fds);
	return (true);
}
bool ft_skip(int *index, char *delimi, char *temp)
{
	if (!ft_strncmp(temp, delimi, ft_strlen(delimi) + 1))
		return (true);
	while (temp[(*index)] && temp[(*index)] != '$')
		(*index)++;
	if (temp[(*index)] && temp[(*index)] != '$')
		(*index)--;
	return (false);
}

bool check_ambiguous(char *file)
{
	char **split;

	split = ft_split(file, ' ');
	if (!split)
	{
		g_status = 1;
		ft_putstr_fd("minishell: ambiguous redirect\n",2);
		return (true);
	}
	if (split && split[0] && split[1])
	{
		g_status = 1;
		ft_putstr_fd("minishell: ambiguous redirect\n",2);
		return (free_char_arr(split), true);
	}
	else
		return (free_char_arr(split), false);
}

bool open_redir_in(t_exec *cmd, int *fds)
{
	int i;

	i = -1;
	while (cmd && cmd->redir_in && cmd->redir_in[++i])
	{
		if (cmd->ambiguous)
		{
			if (check_ambiguous(cmd->redir_in[i])) // exit status in parsing in case its empty env variable
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
	int i;

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
int *open_redir(t_exec *cmd)
{
	int *fds;

	fds = malloc(sizeof(int) * 2);
	fds[1] = -1;
	fds[0] = -1;
	if (!open_redir_in(cmd, fds))
		return (NULL);
	if (!open_redir_out(cmd, fds))
		return (NULL);
	return (fds);
}

bool if_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strncmp(cmd, "cd", 3))
		return (true);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (true);
	if (!ft_strncmp(cmd, "echo", 5))
		return (true);
	if (!ft_strncmp(cmd, "env", 4))
		return (true);
	if (!ft_strncmp(cmd, "export", 7))
		return (true);
	if (!ft_strncmp(cmd, "unset", 6))
		return (true);
	if (!ft_strncmp(cmd, "exit", 5))
		return (true);
	return (false);
}

void ft_clear(int cmd_num, t_fd *fd, int *pids)
{
	int i;
	int exit_status;
	int status;

	i = 0;
	exit_status = g_status;
	ft_close(cmd_num, fd->pipes, NULL);
	while (i < cmd_num)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				exit_status = 130;
		}
		i++;
	}
	free(pids);
	free(fd->fds);
	free_int(fd->pipes, cmd_num);
	free(fd);
	g_status = exit_status;
}

int *ft_open(t_exec *cmd)
{
	int heredoc;
	int *fds;
	int *pip;

	heredoc = 0;
	pip = malloc(sizeof(int) * 2);
	if (!pip)
		return (NULL);
	if (cmd->heredoc_end)
		if_herdoc(cmd->heredoc_end, &heredoc , cmd, pip);
	free(pip);
	fds = open_redir(cmd);
	if (!fds)
		return (NULL);
	if (cmd->heredoc)
		fds[0] = heredoc;
	return (fds);
}

int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

bool ft_path(t_env *env)
{
	while (env)
	{
		if ((ft_strncmp(env->var, "PATH", 5) == 0) && env->value != NULL)
			return (true);
		env = env->next;
	}
	return (false);
}

void ft_print_error(char *message, int exit_status)
{
	if (!message)
		perror("minishell");
	else
		ft_putstr_fd(message, 2);
	exit(exit_status);
}
void ft_exec_error(t_exec *cmd)
{
	if (is_directory(cmd->path_option_args[0]) == 1)
		ft_print_error("minishell: is a directory\n", 126);
	else if (errno == EACCES)
		ft_print_error(NULL, 126);
	else if (!ft_path(cmd->env))
		ft_print_error("minishell :No such file or directory\n", 127);
	else if (errno == ENOENT)
		ft_print_error("minishell :command not found\n", 127);
	else
		ft_print_error(NULL, 1);
}
void init_fds(int **pids, t_fd **fd, int cmd_num)
{
	*pids = malloc(sizeof(int) * cmd_num);
	*fd = malloc(sizeof(t_fd));
	(*fd)->pipes = ft_pip(cmd_num);
	if (!(*fd)->pipes)
		return (free(*pids), free(*fd));
}
void free_fds(int *pids, t_fd **fd, int cmd_num)
{
	free(pids);
	free_int((*fd)->pipes, cmd_num);
	free((*fd)->fds);
	free(*fd);
}
void close_fds(t_fd **fd)
{
	close ((*fd)->fds[0]);
	close ((*fd)->fds[1]);
	free((*fd)->fds);
	(*fd)->fds = NULL;
}
void ft_run_cmd(t_exec *cmd, t_env ***env, t_fd *fd, int i)
{
	char **strenv;
	int cmd_num = ft_count_cmd(cmd);
	if (cmd_num > 0)
		cmd_num += i;
	strenv = env_to_str(**env);
	if (g_status == 300)
		exit(1);
	fd_hindler(cmd_num, fd->pipes, fd->fds, i);
	if (cmd->path_option_args && if_builtin(cmd->path_option_args[0]))
	{
		if (if_builtin(cmd->path_option_args[0]) && !ft_builtin(cmd, *env, 1))
			exit(1);
		else
			exit(0);
	}
	else if (execve(cmd->path_option_args[0], cmd->path_option_args, strenv) == -1)
	{
		free_char_arr(strenv);
		ft_exec_error(cmd);
	}
}
bool run_one_builtin(t_exec *cmds, t_env ***env, t_fd *fd, int cmd_num)
{
	if (cmd_num == 1 && if_builtin(cmds->path_option_args[0]))
	{
		if (!ft_builtin(cmds, *env, fd->fds[1]))
			g_status = 1;
		close_fds(&fd);
		return (true);
	}
	return (false);
}
void ft_exic(t_exec *cmds, t_env **env)
{
	int cmd_num;
	int i;
	int *pids;
	t_fd *fd;

	cmd_num = ft_count_cmd(cmds);
	i = -1;
	fd = NULL;
	init_fds(&pids, &fd, cmd_num);
	while (++i < cmd_num)
	{
		fd->fds = ft_open(cmds);
		if (!fd->fds || !cmds->path_option_args[0])
			return (free_fds(pids, &fd, cmd_num));
		if (run_one_builtin(cmds, &env, fd, cmd_num))
			break ;
		pids[i] = fork();
		if (pids[i] == -1)
			return (free_fds(pids, &fd,cmd_num));
		if (pids[i] == 0)
			ft_run_cmd(cmds, &env, fd, i);
		cmds = cmds->next;
		close_fds(&fd);
	}
	ft_clear(cmd_num ,fd, pids);
}
