/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 08:58:47 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/10 12:01:33 by adbouras         ###   ########.fr       */
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

typedef enum e_token
{
	WORD = 0,
	W_SPACE = ' ',
	D_QUOTE = '\"',
	S_QUOTE = '\'',
	ESCAPE = '\\',
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

typedef struct s_data
{
	t_elem	*head;
	t_elem	*tail;
	int		size;
}	t_data;

typedef struct s_exec
{
	char				**path_option_args;	// /bin/ls -la libft
	char				**redir_in;			// files_name 
	char				**redir_out;			// files_name
	char				**heredoc_end;			// delimiter
	bool				append;				// last >>
	bool				heredoc;				// last <<
	struct s_exec		*next;
}	t_exec;

/***	utils.c				***********************************************/
void	init_data(t_data **tokens);
char	*ft_strndup(const char *s1, int n);
char	*remove_spaces(char *str);		// ---------------	 REMOVE LATER!!

/***	list_utils.c		***********************************************/
t_elem	*skip_wspace(t_elem *token, char direction);
t_elem	*new_token(char *content, int i, t_token type, t_state state);
t_elem	*last_token(t_elem *token);		// ---------------	 REMOVE LATER!!
void	token_add_back(t_data *token, t_elem *new);

/***	parse_utils.c		***********************************************/
bool	is_white_space(char c);
bool	is_grammar(char c);
bool	is_red(t_token type);

/***	lexer.c				***********************************************/
void	ft_lexing(char *read, t_data **tokens);
int		ft_tokenize(t_data *tokens, char *read, int i, t_state *status);
int		get_word(t_data *tokens, char *read, t_state status);
int		red_token(t_data *tokens, char *read, int i, t_state *status);
void	quote_token(t_data *tokens, char *read, t_token type, t_state *status);

/***	syntax.c			***********************************************/
bool	if_syntax_err(t_data *tokens);
bool	if_closed_quotes(t_elem **token, t_token type);
bool	red_syntax(t_elem *token);

/***	parse_utils.c			*******************************************/
bool	is_red(t_token type);
bool	is_grammar(char c);
bool	is_white_space(char c);

/***	clean.c				***********************************************/
void	free_tokens(t_data **tokens);

/***	error.c				***********************************************/
void	ft_exit(t_data **tokens, char *err);
void	ft_error(char *err);

/***	print.c				***********************************************/
void	print_tokens(t_data *tokens);
char	*state_to_string(t_state state);
char	*token_to_string(t_token token);

#endif