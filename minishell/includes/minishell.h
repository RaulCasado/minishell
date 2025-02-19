
#ifndef MINISHELL_H
# define MINISHELL_H

# define SIMPLE_MARK '\''
# define DOUBLE_MARK '\"'
# define DOLLAR '$'
# define INTERROGATION '?'
# define HYPHEN '-'

# define BUFFER_SIZE 1024

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

typedef struct s_command
{
	// something like this is needed
	char **args; // ["ls", "-l" "-h", NULL]
	int pipe_in; // STDIN_FILENO
	int pipe_out; // STDOUT_FILENO
	// here we can add more stuff like redirections
	char *infile; // if this exists we need to redirect stdin to this file
	char *outfile; // if this exists we need to redirect stdout to this file
	// if outfile exists we should check if it is going to be appended
	int append; // if this is 1 we need to append to the file instead of overwriting it (0)
	struct s_command *next;

}	t_command;

typedef struct s_minishell
{
	t_token		*tokens;
	t_command	*commands;
}	t_minishell;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "libft/libft.h"

/*	Tokenize	*/
t_token	*tokenize_input(char *input);
void	expand_tokens(t_token **tokens);

/*	Parse	*/
int	tokenize_check(t_token *tokens);

/* */
void	free_tokens(t_token *tokens);
t_token	*create_token(char *value);
void	add_token(t_token **tokens, char *value);

/*	Signals	*/
int	get_signal();
void set_signal(int signal);
void setup_signals(void);
void handle_sigquit();
void handle_sigint(int sig);
void handle_eof();

/*	Token utils */
t_token	*ft_get_last(t_token *token);
t_token	*ft_get_token(t_token *token, size_t i);
size_t	ft_token_len(t_token *token);
char	ft_add_token(t_token **token, t_token *new_token, size_t i);
char	ft_remove_token(t_token **token, size_t i);

/*	Extras	*/
void	print_tokens(t_token *tokens);

/*	Commands	*/
t_command	*parse_tokens(t_token *tokens);
void	print_commands(t_command *commands);
char	command_executer(t_command *commands);

/*	Built-ins	*/
char	builtin_echo(t_command *command);
char	builtin_pwd(t_command *command);

/*	Utils	*/
char	*get_env(char *path);

/*	Str Utils	*/
void	write_strs(char **strs, char newline, char fd);

#endif