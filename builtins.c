/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 13:17:59 by eismail           #+#    #+#             */
/*   Updated: 2024/08/28 09:12:05 by eismail          ###   ########.fr       */
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
		path = home_env->value;
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

bool env_var(t_env *env, char **arg)
{
	int i;
	t_env *temp;

	i = -1;
	temp = env;
	while(arg[++i])
	{
		if (arg[i][0] == '$')
		{
			while (temp)
			{
				if (!ft_strncmp(&arg[i][1], temp->var, ft_strlen(temp->var) + 1))
				{
					
					printf("%s\n",temp->value);
					return (true);
				}
				temp = temp->next;
			}
		}
	}
	return (false);
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
		ft_putstr_fd("\n",fd_out);
	g_status = 0;
	return (true);
}

bool ft_env(t_env *env, int fd_out)
{
	// printf("++++++++++++++\n");
	// printf("[%s]++++++++++++++++\n",env->var);1
	while(env && env->var)
	{
		ft_putstr_fd(env->var, fd_out);
		ft_putstr_fd("=", fd_out);
		ft_putstr_fd(env->value,fd_out);
		ft_putstr_fd("\n", fd_out);
		env = env->next;
	}
	return (true);
}

bool cheak_var(char *var)
{
	int i;

	i = -1;
	while (var[++i])
	{
		if (var[i] != '_' && !ft_isalnum(var[i])) //!@#$%^&*()-=+
		{
			g_status = 1;
			fprintf(stderr, "minishell: export: not a valid identifier\n");
			return (false);
		}
	}
	if (var && var[0] != '\0' && var[0] != '_')
	{
		if (!ft_isalpha(var[0])) //123var
		{
			g_status = 1;
			fprintf(stderr, "minishell: export: not a valid identifier\n");
			return (false);
		}
	}
	else 
		return (false);
	return (true);
}

bool update_var(t_env **env, char *arg, char *new_var)
{
	t_env *temp;
	t_env *new;
	
	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(new_var,temp->var,ft_strlen(new_var) + 1))
		{
			new = creat_var(arg);
			temp->value = new->value;
			g_status = 0;
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
		while (env)
		{
			dprintf(fd_out, "declare -x %s=%s\n", env->var, env->value);
			env = env->next;
		}
		g_status = 0;
		return (true);
	}
	// if (arg && *arg && arg[0][0] == '\0')
	// {
	// 	while (env)
	// 	{
	// 		dprintf(fd_out, "declare -x %s=%s\n", env->var, env->value);
	// 		env = env->next;
	// 	}
	// 	g_status = 0;
	// 	return (true);
	// }
	return (false);
}

bool ft_export(t_env **env, char **arg, int fd_out)
{
	int i;
	int j;
	char *new_var;
	t_env *new;

	j = -1;
	(void) fd_out;
	if (export_no_arg(*env, arg, fd_out))
		return (true);
	while (arg[++j])
	{
		i = 0;
		while (arg && arg[j] && arg[j][i] != '\0' && arg[j][i] != '=')
			i++;
		new_var = ft_substr(arg[j], 0, i);
		if (!cheak_var(new_var))
			return (true);
		if (update_var(env, arg[j], new_var))
			continue;
		new = creat_var(arg[j]);
		add_env(env, new);
		free(new_var);
	}
	g_status = 1;
	return (true);
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
				if (!prev)
					*env = temp->next;
				else
					prev->next = temp->next;
				free(temp->var);
				free(temp->value);
				free(temp);
				temp = NULL;
				g_status = 0;
				return (true);
			}
			prev = temp;
			temp = temp->next;
		}
		g_status = 0;
	}
	return (true);
}

bool ft_exit_built(char **arg)
{
	int i;

	i = -1;
	if (!arg || !*arg)
		exit(0);
	while (arg && *arg && arg[0][++i])
	{
		if (!ft_isdigit(arg[0][i]))
		{
			printf("bash: exit: numeric argument required");
			exit (255);
		}	
	}
	if (arg && *arg && arg[1])
	{
		printf("bash: exit: too many arguments");
		return (true);
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
	dprintf(fd_out, "%s\n",s);
	g_status = 0;
	return (true);
}

bool ft_builtin(t_exec *cmd, t_env **envi, int fd_out)
{
	char *builtin;

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
		return (ft_exit_built(&cmd->path_option_args[1]));
	return (false);
}
