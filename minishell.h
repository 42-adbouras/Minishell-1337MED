/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/09/02 12:27:23 by eismail          ###   ########.fr       */
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
# include <curses.h>
# include <term.h>
# include <errno.h>
# include <sys/stat.h>
# include "libft/libft.h"

# define BBLK "\e[1;30m"
# define BRED "\e[1;31m"
# define BGRN "\e[1;32m"
# define BYEL "\e[1;33m"
# define BBLU "\e[1;34m"
# define BMAG "\e[1;35m"
# define BCYN "\e[1;36m"
# define RSET "\e[0m"

# define PROMPT "~ "

/****************************	adbouras	****************************/

typedef struct sigaction t_sa;

typedef enum e_token
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
}	t_token;

typedef enum e_state
{
	IN_DQUOTE,
	IN_SQUOTE,
	GENERAL,
}	t_state;

typedef struct s_elem
{
	char			*content;
	int				len;
	enum e_token	type;
	enum e_state	state;
	struct s_elem	*next;
	struct s_elem	*prev;
}	t_elem;

typedef struct s_env
{
    char *var;
    char *value;
    struct s_env *next;
    
}   t_env;

typedef struct s_exec
{
	char			**path_option_args;	// /bin/ls -la libft
	char			**redir_in;			// files_name 
	char			**redir_out;			// files_name
	char			**heredoc_end;			// delimiter
	bool			append;				// last >>
	bool			heredoc;			// last <<
	bool			exed;
	bool			run;
	struct s_exec	*next;
	struct s_env	*env;
}	t_exec;

typedef struct s_data
{
	t_elem	*head;
	t_exec	*exec;
}	t_data;
typedef struct s_fd
{
	int **pipes;
	int *fds;
}	t_fd;

/***	ft_readline.c		***********************************************/
char	*get_prompt();
char	*ft_getcwd();
int		ft_readline(char **rl);

/***	utils.c				***********************************************/
void	init_data(t_data **tokens);
char	*ft_strndup(const char *s1, int n);

/***	arg_utils.c			***********************************************/
char	*get_cmd(t_elem *tokens, t_env *env, bool *exed);
int		count_words(t_elem *tokens);
char	*get_access(char *cmd, t_env *env);
char	*get_spichil(t_elem **temp, char **new, t_env *env);

/***	lexer_utils.c		***********************************************/
bool	is_white_space(char c);
bool	is_grammar(char c);
bool	is_red(t_token type);
t_elem	*skip_wspace(t_elem *token, char direction);

/***	lexer_list_utils.c		*******************************************/
t_elem	*new_token(char *content, int i, t_token type, t_state state);
t_elem	*last_token(t_elem *token);
void	token_add_back(t_data *tokens, t_elem *new);

/***	lexer.c				***********************************************/
void	ft_lexing(char *read, t_data **tokens);
int		ft_tokenize(t_data *tokens, char *read, int i, t_state *status);
int		get_word(t_data *tokens, char *read, t_state status);
int		red_token(t_data *tokens, char *read, int i, t_state *status);
void	quote_token(t_data *tokens, char *read, t_token type, t_state *status);

/***	syntax.c			***********************************************/
bool	pipe_syntax(t_elem *token);
bool	if_syntax_err(t_data *tokens);
bool	if_closed_quotes(t_elem **token, t_token type);
bool	red_syntax(t_elem *token);

/***	parse_utils.c			*******************************************/
bool	process_redir(t_elem *tokens, t_exec **new, t_env *env);
char 	*process_expander(t_elem **temp, t_env *env);
char	*ft_expand(t_env *env, char *var);
char	*get_arg(t_elem **token, t_env *env, bool exec);

/***	parse_list_utils.c			***************************************/
t_exec	*new_exec(t_elem *tokens, t_env *env);
void    new_exec_node(t_exec **new, t_elem *tokens, t_env *env);
void	init_exec_struct(t_data **data, t_env *env);
void	exec_add_back(t_exec **exec, t_exec *new);

/***	redir_utils.c		***********************************************/
int		count_red(t_elem *tokens, t_token type);
char 	*get_redire(t_elem **token, t_env *env);
char	*get_heredoc(t_elem **token);
bool	last_heredoc(t_elem *token);

/***	signals.c			***********************************************/
void    signals_init();
void	sig_handler(int sig, siginfo_t *siginfo, void *ptr);
void	child_sig_init();

/***	clean.c				***********************************************/
void	free_data(t_data **data, char **rl, int flag);
void	free_tokens(t_elem **tokens);
void	free_char_arr(char **arr);
char 	*get_redirec(t_elem **token);
bool 	last_heredoc(t_elem *token);
void	free_exec(t_exec **exec);
void	delete_token(t_elem **token);

/***	error.c				***********************************************/
void	ft_exit(t_data **tokens, char *err);
bool	ft_error(char *err);

/***	print.c				***********************************************/
void	print_tokens(t_data *tokens);
char	*state_to_string(t_state state);
char	*token_to_string(t_token token);

/***	signls.c				***********************************************/
void    signals_init();
void	sigint_handler(int sig);
void	sigquit_handler(int sig);

void	remove_quotes(t_elem **tokens);
void child_process_code();
void handle_sigint(int sig);

/****************************		eismail		****************************/
int g_status;

# define MAX_PATH 1024

t_env *creat_var(char *var);
t_env *set_env(char **env);
void add_env(t_env **head, t_env *env_new);
void free_env(t_env **env);
void ft_exic(t_exec *cmds, t_env **envi);
bool ft_builtin(t_exec *cmd, t_env **envi, int fd_out);
bool    ft_cd(char *path, t_env *env);
// bool env_var(t_env *env, char **arg);
int echo_option(t_exec *cmd);
bool ft_echo(t_exec *cmd, int fd_out);
bool ft_env(t_env *env, int fd_out);
bool cheak_var(char *var);
bool update_var(t_env **env, char *arg, char *new_var);
bool export_no_arg(t_env *env, char **arg, int fd_out);
bool ft_unset(t_env **env, char **toDelete);
bool ft_exit_built(char **arg, int cmd_num);
int ft_count_cmd(t_exec *cmd);
void	free_int(int **p, int n);
int	**ft_pip(int cmd_num);
void	ft_close(int cmd_num, int **pipes, int *fds);
void	ft_stdin(int **pipes, int pid, int *fds);
void	ft_stdout(int cmd_num, int **pipes, int pid, int *fds);
bool fd_hindler(int cmd_num, int **fd, int  *fds, int pos);
void read_heredoc(char *delimiter, int *pip, t_env *env);
void if_herdoc(char **delimiters, int *fd_heredoc, t_env *env);
int *open_redir(t_exec *cmd);
bool if_builtin(char *cmd);
void ft_clear(int cmd_num, t_fd *fd, int *pids);
bool ft_pwd(int fd_out);
bool ft_export(t_env **env, char **arg, int fd_out);
char **env_to_str(t_env *env);
char *arg_expand(t_elem *token, t_env *env, char **arg);
void if_redir(t_elem **token);

#endif
