
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
	//TOKEN_COMMAND, // maybe join it with TOKEN_WORD because its difficult to differentiate
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
#include <signal.h>
#include "libft/libft.h"

t_token	*tokenize_input(char *input);
void	free_tokens(t_token *tokens);
t_token	*create_token(char *value);
void	add_token(t_token **tokens, char *value);

void	print_tokens(t_token *tokens);

#endif