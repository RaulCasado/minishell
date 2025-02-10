/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:33:10 by racasado          #+#    #+#             */
/*   Updated: 2025/02/10 19:50:56 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_token_type
{
	TOKEN_WORD, // normal word
	TOKEN_PIPE, // |
	TOKEN_REDIR_IN, // <
	TOKEN_REDIR_OUT, // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC, // <<
	// we could add TOKEN_ENV_VAR OR TOKEN_FLAG but right now its in TOKEN_WORD
	TOKEN_COMMAND, // maybe join it with TOKEN_WORD because its difficult to differentiate
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "libft/libft.h"

#endif