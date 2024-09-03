/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 13:17:59 by eismail           #+#    #+#             */
/*   Updated: 2024/09/03 13:29:07 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_error(void)
{
	ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
}

bool	cheak_var(char *var)
{
	int	i;

	i = -1;
	g_status = 1;
	while (var[++i])
	{
		if (var[i] != '_' && !ft_isalnum(var[i]))
			return (export_error(), false);
	}
	if (var && var[0] != '\0' && var[0] != '_')
	{
		if (!ft_isalpha(var[0]))
			return (export_error(), false);
	}
	else if (var && var[0] == '\0')
		return (export_error(), false);
	g_status = 0;
	return (true);
}

bool	update_var(t_env **env, char *arg, char *new_var)
{
	t_env	*temp;
	t_env	*new;

	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(new_var, temp->var, ft_strlen(new_var) + 1))
		{
			if (!ft_strchr(arg, '='))
				return (true);
			new = creat_var(arg);
			free(temp->value);
			temp->value = NULL;
			temp->value = ft_strdup(new->value);
			free(new->var);
			free(new->value);
			free(new);
			return (true);
		}
		temp = temp->next;
	}
	return (false);
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

bool	ft_builtin(t_exec *cmd, t_env **envi, int fd_out)
{
	char	*builtin;
	int		cmd_num;

	cmd_num = ft_count_cmd(cmd);
	if (!cmd || !cmd->path_option_args)
		return (false);
	if (fd_out == -1)
		fd_out = 1;
	builtin = cmd->path_option_args[0];
	if (!ft_strncmp(builtin, "cd", 3) || !ft_strncmp(builtin, "CD", 3))
		return (ft_cd(cmd->path_option_args[1], *envi));
	if (!ft_strncmp(builtin, "pwd", 4) || !ft_strncmp(builtin, "PWD", 4))
		return (ft_pwd(fd_out));
	if (!ft_strncmp(builtin, "echo", 5) || !ft_strncmp(builtin, "ECHO", 5))
		return (ft_echo(cmd, fd_out));
	if (!ft_strncmp(builtin, "env", 4) || !ft_strncmp(builtin, "ENV", 4))
		return (ft_env(cmd->env, fd_out));
	if (!ft_strncmp(builtin, "export", 7) || !ft_strncmp(builtin, "EXPORT", 7))
		return (ft_export(envi, &cmd->path_option_args[1], fd_out));
	if (!ft_strncmp(builtin, "unset", 6) || !ft_strncmp(builtin, "UNSET", 6))
		return (ft_unset(envi, &cmd->path_option_args[1]));
	if (!ft_strncmp(builtin, "exit", 5) || !ft_strncmp(builtin, "EXIT", 5))
		return (ft_exit_built(&cmd->path_option_args[1], cmd_num));
	return (false);
}
