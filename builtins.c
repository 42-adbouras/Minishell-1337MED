/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 13:17:59 by eismail           #+#    #+#             */
/*   Updated: 2024/09/02 12:35:44 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    ft_cd(char *path, t_env *env)
{
	t_env *home_env;

	home_env = env;
	while (home_env && ft_strncmp(home_env->var, "HOME", 5))
		home_env = home_env->next;
	if (path == NULL)
	{
		path = &home_env->value[1];
		chdir(path);
	}
	if (chdir(path) != 0)
	{
		if (errno != ENOENT)
			fprintf(stderr, "minishell: cd : Permission denied\n");
		else
			perror("minishell: cd ");
		g_status = 1;
		return (true);
	}
	g_status = 0;
	return (true);
}


int echo_option(t_exec *cmd)
{
	int i;
	int j;
	
	i = 0;
	j = -1;
	if (!cmd->path_option_args)
		return (0);
	while(cmd->path_option_args[++i])
	{
		j = 0;
		if (cmd->path_option_args[i][0] == '-')
		{
			while (cmd->path_option_args[i][++j])
			{
				if (cmd->path_option_args[i][j] != 'n')
					return (i);
			}
		}
		else
			break;
	}
	return (i);
}

bool ft_echo(t_exec *cmd, int fd_out)
{
	int i;

	i = echo_option(cmd) - 1;
	while (cmd->path_option_args[i + 1] && cmd->path_option_args[++i])
	{
		ft_putstr_fd(cmd->path_option_args[i], fd_out);
		if (cmd && cmd->path_option_args[i] && cmd->path_option_args[i + 1])
			ft_putstr_fd(" ", fd_out);
	}
	if (echo_option(cmd) - 1 == 0)
		ft_putstr_fd("\n", fd_out);
	g_status = 0;
	return (true);
}

bool ft_env(t_env *env, int fd_out)
{
	while(env)
	{
		if (ft_strchr(env->value, '='))
		{
			ft_putstr_fd(env->var, fd_out);
			ft_putstr_fd(env->value, fd_out);
			ft_putstr_fd("\n", fd_out);
		}
		env = env->next;
	}
	return (true);
}

void export_error()
{
	ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
}

bool cheak_var(char *var)
{
	int i;

	i = -1;
	g_status = 1;
	while (var[++i])
	{
		if (var[i] != '_' && !ft_isalnum(var[i])) //!@#$%^&*()-=+
			return (export_error(), false);
	}
	if (var && var[0] != '\0' && var[0] != '_')
	{
		if (!ft_isalpha(var[0])) //123var
			return (export_error(), false);
	}
	else if (var && var[0] == '\0')
		return (export_error(), false);
	g_status = 0;
	return (true);
}

bool update_var(t_env **env, char *arg, char *new_var)
{
	t_env *temp;
	t_env *new;
	
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

bool export_no_arg(t_env *env, char **arg , int fd_out)
{
	if (!arg || !*arg)
	{
		while(env)
		{
			ft_putstr_fd("declare -x ", fd_out);
			ft_putstr_fd(env->var, fd_out);
			if (env->value && env->value[0] == '=')
			{
				ft_putstr_fd("=", fd_out);
				ft_putstr_fd("\"", fd_out);
				ft_putstr_fd(&env->value[1],fd_out);
				ft_putstr_fd("\"", fd_out);
			}
			ft_putstr_fd("\n", fd_out);
			env = env->next;
		}
		g_status = 0;
		return (true);
	}
	return (false);
}

bool free_new_var(char **s)
{
	free(*s);
	*s = NULL;
	return (true);
}

bool ft_export(t_env **env, char **arg, int fd_out)
{
	int i;
	int j;
	char *new_var;
	t_env *new;

	j = -1;
	if (export_no_arg(*env, arg, fd_out))
		return (true);
	while (arg[++j])
	{
		i = 0;
		while (arg && arg[j] && arg[j][i] != '\0' && arg[j][i] != '=')
			i++;
		new_var = ft_substr(arg[j], 0, i);
		if (!cheak_var(new_var) && free_new_var(&new_var))
			continue;
		if (update_var(env, arg[j], new_var) && free_new_var(&new_var))
			continue;
		new = creat_var(arg[j]);
		add_env(env, new);
		free_new_var(&new_var);
	}
	if (new_var)
		free(new_var);
	return (true);
}

void remove_var(t_env **prev, t_env **var, t_env ***env)
{
	if (!*prev)
		**env = (*var)->next;
	else
		(*prev)->next = (*var)->next;
	free((*var)->var);
	free((*var)->value);
	free(*var);
	*var = NULL;
}

bool ft_unset(t_env **env, char **toDelete)
{
	t_env *temp;
	t_env *prev;
	int i;
	
	i = -1;
	while (toDelete[++i])
	{
		if (!cheak_var(toDelete[i]))
			return (true);
		temp = *env;
		prev = NULL;
		while (temp)
		{
			if (!ft_strncmp(toDelete[i],temp->var,ft_strlen(temp->var) + 1))
			{
				remove_var(&prev, &temp, &env);
				return (true);
			}
			prev = temp;
			temp = temp->next;
		}
	}
	return (true);
}

bool ft_exit_built(char **arg, int cmd_num)
{
	int i;

	i = -1;
	if (!arg || !*arg)
	{
		if (cmd_num > 1)
			exit(0);
		exit(g_status);
	}
	while (arg && *arg && arg[0][++i])
	{
		if (!ft_isdigit(arg[0][i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			exit (255);
		}	
	}
	if (arg && *arg && arg[1])
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (false);
	}
	else
		exit(ft_atoi(arg[0]));
}

bool ft_pwd(int fd_out)
{
	char s[MAX_PATH];
	if (getcwd(s, MAX_PATH) == NULL)
	{
		g_status = 1;
		return (true);
	}
	ft_putstr_fd(s, fd_out);
	ft_putstr_fd("\n", fd_out);
	g_status = 0;
	return (true);
}

bool ft_builtin(t_exec *cmd, t_env **envi, int fd_out)
{
	char *builtin;
	int cmd_num;

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
