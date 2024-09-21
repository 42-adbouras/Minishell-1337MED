/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/21 17:17:18 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "libft/libft.h"

# define MAX_PATH 1024

int							g_status;

/****************************	adbouras	****************************/

typedef struct sigaction	t_sa;

typedef enum e_type
{
	WORD = 0,
	W_SPACE = ' ',
	D_QUOTE = '\"',
	S_QUOTE = '\'',
	PIPE = '|',
	REDIR_IN = '<',
	REDIR_AND,
	REDIR_OUT = '>',
	REDIR_APP,
	ENV = '$',
}	t_type;

typedef enum e_state
{
	IN_DQUOTE,
	IN_SQUOTE,
	GENERAL,
}	t_state;

typedef struct s_token
{
	char			*content;
	int				len;
	t_type			type;
	t_state			state;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_exec
{
	char			**path_option_args;
	char			**redir_in; 
	char			**redir_out;
	char			**heredoc_end;
	bool			append;
	bool			heredoc;
	bool			exed;
	bool			run;
	bool			expand_heredoc;
	bool			ambiguous;
	struct s_exec	*next;
	struct s_env	*env;
}	t_exec;

typedef struct s_data
{
	t_token	*tokens;
	t_exec	*exec;
}	t_data;

typedef struct s_fd
{
	int		**pipes;
	int		**fds;
}	t_fd;

/***	ft_readline.c		***********************************************/
char	*get_prompt(void);
char	*ft_getcwd(void);
int		ft_readline(char **rl);

/***	utils.c				***********************************************/
void	init_data(t_data **tokens, char *rl);
char	*ft_strndup(const char *s1, int n);
void	skip_quotes(t_token ***token, t_state *state);
bool	skip_cmd(t_exec **cmd, int *arry);

/***	arg_utils.c			***********************************************/
char	*get_cmd(t_token *tokens, t_env *env, bool *exed);
int		count_words(t_token *tokens);
char	*get_access(char *cmd, t_env *env);
char	*get_spichil(t_token **temp, char **new, t_env *env);

/***	lexer_utils.c		***********************************************/
bool	is_white_space(char c);
bool	is_grammar(char c);
bool	is_red(t_type type);
t_token	*skip_wspace(t_token *token, char direction);

/***	lexer_list_utils.c		*******************************************/
t_token	*new_token(char *content, int i, t_type type, t_state state);
t_token	*last_type(t_token *token);
void	token_add_back(t_data *tokens, t_token *new);

/***	lexer.c				***********************************************/
void	ft_lexing(char *read, t_data **tokens);
int		ft_typeize(t_data *tokens, char *read, int i, t_state *status);
int		get_word(t_data *tokens, char *read, t_state status);
int		red_token(t_data *tokens, char *read, int i, t_state *status);
void	quote_token(t_data *tokens, char *read, t_type type, t_state *status);

/***	syntax.c			***********************************************/
bool	pipe_syntax(t_token *token);
bool	if_syntax_err(t_data *tokens);
bool	if_closed_quotes(t_token **token, t_type type);
bool	red_syntax(t_token *token);

/***	parse_utils.c			*******************************************/
bool	process_redir(t_token *tokens, t_exec **new, t_env *env);
char	*ft_expander(t_token **temp, t_env *env, bool exec);
char	*ft_expand(t_env *env, char *var);
char	*get_arg(t_token **token, t_env *env, bool exec);
void	init_exec_node(t_exec **new, t_token *tokens, t_env *env);
void	rest_function(t_token **token, t_state *state);

/***	parse_list_utils.c			***************************************/
t_exec	*new_exec(t_token *tokens, t_env *env);
void	new_exec_node(t_exec **new, t_token *tokens);
void	init_exec_struct(t_data **data, t_env *env);
void	exec_add_back(t_exec **exec, t_exec *new);

/***	expand_utils.c		***********************************************/
char	*arg_expand(t_token *token, t_env *env, char **arg);
char	*check_exec(bool exec, char **arg, char **join, t_env *env);
char	*ft_expand(t_env *env, char *var);

/***	redir_utils.c		***********************************************/
int		count_red(t_token *tokens, t_type type);
char	*get_redire(t_token **token, t_env *env, bool *ambiguous);
char	*get_heredoc(t_token **token, bool *heredoc);
bool	last_heredoc(t_token *token);

/***	signals.c			***********************************************/
void	signals_init(void);
void	sig_handler(int sig, siginfo_t *siginfo, void *ptr);

/***	clean.c				***********************************************/
void	free_data(t_data **data, char **rl, int flag);
void	free_tokens(t_token **tokens);
void	free_char_arr(char **arr);
char	*get_redirec(t_token **token);
bool	last_heredoc(t_token *token);
void	free_exec(t_exec **exec);
void	delete_token(t_token **token);

/***	process_redir_utils.c	*******************************************/
bool	redir_conditions(t_token *temp, int flag);
bool	get_redir_in(t_exec ***new, t_token *temp, t_env *env, int *i);
void	if_redir(t_token **token);

/***	word_count_utils.c 		*******************************************/
bool	not_pipe(t_token *tokens);
bool	should_skip(t_token *tokens);
bool	is_word(t_token *tokens);
bool	increment(t_token *tokens);

/***	getters.c			***********************************************/
bool	cmd_getter(t_token *temp, t_exec *new);
bool	arg_getter(t_token *temp);
bool	get_redir_out(t_exec ***new, t_token *temp, t_env *env, int *j);
void	heredoc_getter(t_exec ***new, t_token *temp, int *l);
char	*get_exit_status(char *str);

/***	error.c				***********************************************/
void	ft_exit(t_data **tokens, char *err);
bool	ft_error(char *err, int exit);

/***	print.c				***********************************************/
void	print_types(t_data *tokens);
char	*state_to_string(t_state state);
char	*token_to_string(t_type token);
void	print_exec(t_exec *exec);

/***	signls.c			***********************************************/
void	signals_init(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	child_signal(int sig);

void	remove_quotes(t_token **tokens);
void	child_process_code(void);
void	handle_sigint(int sig);
char	*get_after(char *var);
char	*get_var(char *var);
char	*arg_join(t_token *token, char **arg, char *join);
void	herdoc_signal(int sig);

/***	word_count_utils.c	************************************************/
bool	not_pipe(t_token *tokens);
bool	should_skip(t_token *tokens);
bool	is_word(t_token *tokens);
bool	increment(t_token *tokens);

/****************************		eismail		****************************/
/***	builtins_utils.c	***********************************************/
void	remove_var(t_env **prev, t_env **var, t_env ***env);
bool	ft_cd(char *path, t_env *env);
int		echo_option(t_exec *cmd);
bool	ft_echo(t_exec *cmd, int fd_out);
bool	ft_unset(t_env **env, char **toDelete);
bool	ft_exit_built(char **arg, int cmd_num);
void	read_heredoc(char *delimiter, int *pip, t_env *env, bool expand);
bool	ft_pwd(int fd_out);
char	*arg_expand(t_token *token, t_env *env, char **arg);
void	if_redir(t_token **token);

/***	ft_unset.c			***********************************************/
bool	ft_skip(int *index, char *delimi, char *temp);
int		is_directory(const char *path);
bool	ft_path(t_env *env);

/***	ft_unset.c			***********************************************/
t_env	*ft_last(t_env *env);
void	free_env(t_env **env);
char	**env_to_str(t_env *env);
bool	init_fds(int **pids, t_fd **fd, int cmd_num);
void	ft_exic(t_exec *cmds, t_env **env);

/***	ft_unset.c			***********************************************/
bool	ft_unset(t_env **env, char **toDelete);
void	remove_var(t_env **prev, t_env **var, t_env ***env);

/***	ft_export.c			***********************************************/
bool	export_no_arg(t_env *env, char **arg, int fd_out);
bool	free_new_var(char **s);
bool	ft_export(t_env **env, char **arg, int fd_out);

/***	ft_env.c			***********************************************/
void	add_env(t_env **tokens, t_env *env_new);
t_env	*creat_var(char *var);
t_env	*set_env(char **env);
bool	ft_env(t_env *env, int fd_out);

/***	ft_open.c			***********************************************/
bool	check_ambiguous(char *file, bool ambiguous);
bool	open_redir_out(t_exec *cmd, int *fds);
bool	open_redir_in(t_exec *cmd, int *fds);
int		*open_redir(t_exec *cmd);
int		*ft_open(t_exec *cmd);

/***	ft_redirection.c	***********************************************/
bool	fd_hindler(int cmd_num, int **fd, int *fds, int pos);
void	ft_stdout(int cmd_num, int **pipes, int pid, int *fds);
void	ft_stdin(int **pipes, int pid, int *fds);
void	ft_close(int cmd_num, int **pipes, int *fds);
int		**ft_pip(int cmd_num, bool pip);

/***	ft_exic_utils.c		***********************************************/
int		ft_count_cmd(t_exec *cmd);
void	ft_exec_error(t_exec *cmd);
void	ft_print_error(char *message, int exit_status);
bool	run_one_builtin(t_exec *cmds, t_env ***env, t_fd *fd, int cmd_num);
void	ft_run_cmd(t_exec *cmd, t_env ***env, t_fd *fd, int i);

/***	builtins.c			***********************************************/
bool	ft_builtin(t_exec *cmd, t_env **envi, int fd_out);
bool	if_builtin(char *cmd);
bool	update_var(t_env **env, char *arg, char *new_var);
bool	cheak_var(char *var);
void	export_error(void);

/***	ft_close_utils.c	***********************************************/
void	free_fds(int *pids, t_fd **fd, int cmd_num);
void	free_int(int **p, int n);
void	close_fds(t_fd **fd, int cmd_num);
void	ft_clear(int cmd_num, t_fd *fd, int *pids);
void	ft_close_pip(int **pip, int i);

/***	heredoc.c			***********************************************/
char	*get_expand_heredoc(char *temp, t_env *env, int i, char **join);
char	*expand_heredoc(char **line, t_env *env, bool expand, char *delimi);
void	read_heredoc(char *delimiter, int *pip, t_env *env, bool expand);
bool	wait_heredoc(int pid, int *pip, char **delimiters, int i);
void	if_herdoc(char **delimiters, int *fd_heredoc, t_exec *cmd, int *pip);

#endif
