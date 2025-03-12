#ifndef MINISHELL_H
# define MINISHELL_H

# define SIMPLE_MARK '\''
# define DOUBLE_MARK '\"'
# define DOUBLE_MARK_STR "\""
# define DOUBLE_DOUBLE_MARK_STR "\"\""
# define DOLLAR '$'
# define QUESTION_MARK '?'
# define HYPHEN '-'

# define PWD "PWD"

# define ECHO_LOWER "echo"
# define CD_LOWER "cd"
# define PWD_LOWER "pwd"
# define EXPORT_LOWER "export"
# define UNSET_LOWER "unset"
# define ENV_LOWER "env"
# define EXIT_LOWER "exit"

# define CWD_SIZE 1024

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h> // needed for wait_pid
# include <errno.h>

typedef enum e_token_type
{
	TOKEN_WORD, // normal word
	TOKEN_PIPE, // |
	TOKEN_REDIR_IN, // <
	TOKEN_REDIR_OUT, // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC, // <<
	// we could add TOKEN_ENV_VAR OR TOKEN_FLAG
	//	but right now its in TOKEN_WORD
	//TOKEN_COMMAND,
	// maybe join it with TOKEN_WORD because
	// its difficult to differentiate
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
	char				**args; // ["ls", "-l" "-h", NULL]
	int					pipe_in; // STDIN_FILENO
	int					pipe_out; // STDOUT_FILENO
	// here we can add more stuff like redirections
	// if this exists we need to redirect stdin to this file
	char				*infile;
	// if this exists we need to redirect stdout to this file
	char				*outfile;
	// if outfile exists we should check if it is going to be appended
	int					append;
	// if this is 1 we need to append to the file instead of overwriting it (0)
	struct s_command	*next;

}	t_command;

typedef struct s_command_info {
    char    **args;
    char    *infile;
    char    *outfile;
    int     append;
    int     pipe_in;
    int     pipe_out;
}   t_command_info;

typedef struct s_minishell
{
	t_token			*tokens;
	t_command		*commands;
	char			**envp;
	int				exit_code;
}	t_minishell;

/*	Minishell	*/
t_minishell	*minishell_builder(char **envp);
void		minishell_reset_loop(t_minishell *minishell);

/*	Tokenize	*/
t_token		*tokenize_input(char *input, t_minishell *minishell);
void		expand_tokens(t_token **tokens, t_minishell *minishell);

/*	Parse	*/
int			tokenize_check(t_token *tokens);

/* Memory	*/
void		free_envp(char **envp);
void		free_tokens(t_token *tokens);
void		free_token(t_token *token);
void		free_minishell(t_minishell *minishell);
void		free_commands(t_command *commands);
void		free_ptr_ptr(char **ptr, size_t len);

/*	Tokens	*/
void		free_tokens(t_token *tokens);
void		add_token(t_token **tokens, char *value);
void		unquoter(t_token **tokens);

/*	Signals	*/
int			get_signal(void);
void		set_signal(int signal);
void		setup_signals(void);
void		handle_sigquit(int sig);
void		handle_sigint(int sig);
void		handle_eof(void);

/*	Token utils */
t_token		*ft_get_last(t_token *token);
t_token		*ft_get_token(t_token *token, size_t i);
size_t		ft_token_len(t_token *token);
char		ft_add_token(t_token **token, t_token *new_token, size_t i);
char		ft_remove_token(t_token **token, size_t i);

/*	Extras	*/
void		print_tokens(t_token *tokens);

/*	Commands	*/
t_command	*parse_tokens(t_token *tokens);
void		print_commands(t_command *commands);
char		command_executer(t_minishell *minishell);
char		command_process(t_minishell *minishell, t_command *command);

/*	Built-ins	*/
int			builtin_echo(t_minishell *minishell, t_command *command);
int			builtin_cd(t_minishell *minishell, t_command *command);
int			builtin_pwd(t_minishell *minishell, t_command *command);
int			builtin_export(t_minishell *minishell, t_command *command);
int			builtin_unset(t_minishell *minishell, t_command *command);
int			builtin_env(t_minishell *minishell, t_command *command);
char		builtin_exit(t_minishell *minishell, t_command *command);

/*	Utils	*/
char		*get_env(char *name, char **envp);
char		**copy_envp(char **envp);
int			add_or_replace_env_var(t_minishell *minishell, char *var);

/*	Str Utils	*/
int			write_strs(char **strs, char nl, char nll, int fd);
int			write_str(char *str, int fd);

/*	Redirections	*/
int			handle_redirections(t_command *cmd);

#endif