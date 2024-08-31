/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:02:41 by eismail           #+#    #+#             */
/*   Updated: 2024/08/31 15:54:22 by eismail          ###   ########.fr       */
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


void read_heredoc(char *delimiter, int *pip)
{
	char *s;
	char *line;
	
	s = ft_strjoin(delimiter, "\n");
	line = ft_strdup("");
	while (ft_strncmp(line, s, ft_strlen(s) + 1) != 0)
	{
		ft_putstr_fd(line, pip[1]);
		free(line);
		line = readline("> ");
		if (!line)
        {
            // Handle SIGINT signal
            free(s);
            exit(130);
        }
		line = ft_strjoin(line, "\n");
	}
	free(line);
	free(s);
	exit(0);
}

void herdoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		exit(1);
	}
}

void if_herdoc(char **delimiters, int *fd_heredoc)
{
	int i;
	int *pip;
	int pid;

	i = -1;
	if (!delimiters || !(*delimiters) )
	{
		*(fd_heredoc) = -1;
		return ;
	}
	pip = malloc(sizeof(int) * 2);
	while (delimiters[++i])
	{
		if (pipe(pip) == -1)
		{
			*(fd_heredoc) = -1;
			return ;
		}
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, herdoc_signal);
			read_heredoc(delimiters[i], pip);
		}
		waitpid(pid, &g_status, 0);
	}
	close(pip[1]);
	*(fd_heredoc) = pip[0];
	free(pip);
}
int *open_redir(t_exec *cmd)
{
	int *fds;
	int i;

	i = -1;
	fds = malloc(sizeof(int) * 2);
	fds[1] = -1;
	fds[0] = -1;
	while (cmd && cmd->redir_in && cmd->redir_in[++i])
	{
		if (ft_strchr(cmd->redir_in[i], ' '))
		{
			fprintf(stderr, "minishell: ambiguous redirect\n");
			return (NULL);
		}
		fds[0] = open(cmd->redir_in[i], O_RDONLY);
		if (fds[0] == -1)
		{
			perror("minishell");
			return (NULL);
		}
		if (cmd->redir_in[i + 1])
			close (fds[0]);
	}
	i = -1;
	while (cmd && cmd->redir_out && cmd->redir_out[++i])
	{
		if (ft_strchr(cmd->redir_out[i], ' '))
		{
			fprintf(stderr, "minishell: ambiguous redirect\n");
			return (NULL);
		}
		if (cmd->append)
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fds[1] = open(cmd->redir_out[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fds[1] == -1)
		{
			perror("minishell");
			return (NULL);
		}
		if (cmd->redir_out[i + 1])
			close (fds[0]);
	}
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

void ft_clear(int cmd_num, int **fd, int *fds, int *pids)
{
	int i;
	int exit_status;
	int status;

	i = 0;
	exit_status = 0;
	ft_close(cmd_num, fd, fds);
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
	free_int(fd, cmd_num);
	g_status = exit_status;
}

int *ft_open(t_exec *cmd)
{
	int heredoc;
	int *fds;

	heredoc = 0;
	if (cmd->heredoc_end)
		if_herdoc(cmd->heredoc_end, &heredoc);
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
void ft_exec_error(t_exec *cmd)
{
	if (is_directory(cmd->path_option_args[0]) == 1)
	{
		ft_putstr_fd("minishell: is a directory\n", 2);
		exit( 126);
	}
	else if (errno == EACCES)
	{
		perror("minishell");
		exit( 126);
	}
	else if (!ft_path(cmd->env))
	{
		ft_putstr_fd("minishell :No such file or directory\n", 2);
		exit(127);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("minishell :command not found\n", 2);
		exit(127);
	}
	else
	{
		perror("minishell");
		exit(1);
	}
}

void ft_exic(t_exec *cmds, t_env **env)
{
	int cmd_num;
	int i;
	int *pids;
	int **fd;
	int *fds;
	char **strenv;

	cmd_num = ft_count_cmd(cmds);
	pids = malloc(sizeof(int) * cmd_num);
	i = 0;
	if (!pids)
		return ;
	fd = ft_pip(cmd_num);
	strenv = env_to_str(*env);
	while (i < cmd_num)
	{
		fds = ft_open(cmds);
		if (!fds || !cmds->path_option_args[0])
			return (free_int(fd, cmd_num), free(pids));
		if (cmd_num == 1 && if_builtin(cmds->path_option_args[0]))
		{
			ft_builtin(cmds, env, fds[1]);
			free(fds);
			fds = NULL;
			break;
		}
		pids[i] = fork();
		if (pids[i] == -1)
			return (free(fds), free_int(fd, cmd_num), free(pids));
		if (pids[i] == 0) 
		{
			fd_hindler(cmd_num, fd, fds, i);
			if (cmds->path_option_args && ft_builtin(cmds, env, fds[1]))
				exit(0);
			if (execve(cmds->path_option_args[0], cmds->path_option_args, strenv) == -1)
				ft_exec_error(cmds);
		}
		i++;
		cmds = cmds->next;
		free(fds);
		fds = NULL;
	}
	free_char_arr(strenv);
	ft_clear(cmd_num ,fd, fds, pids);
}
