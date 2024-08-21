/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouras <adbouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:34:45 by adbouras          #+#    #+#             */
/*   Updated: 2024/08/19 14:46:01 by adbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    new_exec_node(t_exec **new, t_elem *tokens)
{
	(*new) = malloc(sizeof(t_exec));
	(*new)->path_option_args = malloc(sizeof(char *) * (count_words(tokens) + 1));
	(*new)->redir_in = malloc(sizeof(char *) * (count_red(tokens, REDIR_IN) + 1));
	(*new)->redir_out = malloc(sizeof(char *) * (count_red(tokens, REDIR_OUT) + 1));
	(*new)->heredoc_end = malloc(sizeof(char *) * (count_red(tokens, REDIR_AND) + 1));
	(*new)->append = false;
	(*new)->heredoc = false;
	(*new)->next = NULL;
}

