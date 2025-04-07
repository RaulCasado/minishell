/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racasado <racasado@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:03:37 by racasado          #+#    #+#             */
/*   Updated: 2025/03/14 18:03:37 by racasado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**args;
	int					pipe_in;
	int					pipe_out;
	char				*infile;
	char				*outfile;
	int					append;
	// Added fields for multiple output redirections:
	char				**extra_outfiles;
	int					extra_count;
	struct s_command	*next;
}	t_command;

typedef struct s_command_info
{
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		pipe_in;
	int		pipe_out;
	// Added fields for temporary storage during parsing:
	char	**extra_outfiles;
	int		extra_count;
}	t_command_info;

typedef struct s_minishell
{
	t_token		*tokens;
	t_command	*commands;
	char		**envp;
	int			exit_code;
}	t_minishell;

typedef struct s_marks
{
	char	d;
	char	s;
}	t_marks;

/*	Minishell	*/
t_minishell	*minishell_builder(char **envp);
void		minishell_reset_loop(char *input, t_minishell *minishell);

/*	Tokenize	*/
t_token		*tokenize_input(char *input, t_minishell *minishell);
void		expand_tokens(t_token **tokens, t_minishell *minishell);
char		*expand_variable(char *value, ssize_t start, ssize_t end,
				t_minishell *minishell);
int			split_input(ssize_t i, char *ptr, t_token **tokens);
void		handle_marks(char chr, char *mark_d, char *mark_s);

/*	Parse	*/
int			tokenize_check(t_token *tokens);

/* Memory	*/
void		free_envp(char **envp);
void		free_tokens(t_token *tokens);
void		free_token(t_token *token);
void		free_minishell(t_minishell *minishell);
void		free_commands(t_command *commands);
void		free_ptr_ptr(char **ptr, size_t len);
void		ft_free_split(char **split);

/*	Tokens	*/
void		free_tokens(t_token *tokens);
void		add_token(t_token **tokens, char *value);
void		unquoter(t_token **tokens);

/*	Quotes Utils */
char		get_global_marks(char *ptr, char mark_type);

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
int			command_executer(t_minishell *minishell);
int			command_process(t_minishell *minishell, t_command *command);

/*	Built-ins	*/
int			builtin_echo(t_minishell *minishell, t_command *command);
int			builtin_cd(t_minishell *minishell, t_command *command);
int			builtin_pwd(t_minishell *minishell, t_command *command);
int			builtin_export(t_minishell *minishell, t_command *command);
int			builtin_unset(t_minishell *minishell, t_command *command);
int			builtin_env(t_minishell *minishell, t_command *command);
int			builtin_exit(t_minishell *minishell, t_command *command);

/*	Env	*/
char		*get_env(char *name, char **envp);
char		**copy_envp(char **envp);
int			add_or_replace_env_var(t_minishell *minishell, char *var);

/*	Str Utils	*/
int			write_strs(char **strs, char nl, char nll, int fd);
int			write_str(char *str, int fd);
int			ft_strcmp(const char *s1, const char *s2);
int			is_builtin(char *cmd);

/*	Redirections	*/
int			handle_redirections(t_command *cmd);

/*	Command utils */
char		**add_arg(char **args, char *new_arg);
t_command	*create_command(t_command_info *ci, t_command *next);
void		reset_command_info(t_command_info *ci);

/*	Executer utils */
int			count_commands(t_command *cmd);
int			setup_pipe(int pipe_fd[2]);
int			handle_parent_process(int prev_pipe_in,
				int pipe_fd[2], t_command **cmd);
void		cleanup_fds(int fd1, int fd2);
void		wait_for_children(t_minishell *minishell);
void		execute_command(t_minishell *minishell, t_command *cmd);

/*	Command process utils */
char		*handle_direct_path(char *cmd);
char		**get_path_directories(char **envp);

/*	Symbol */
ssize_t		ft_symbol_len(char *ptr);

int			immediate_open_output_file(char *file, int append);
void		handle_redir_in(t_command_info *ci, t_token *next);
void		handle_redir_out(t_command_info *ci, t_token *next, int append);
char		**add_arg(char **args, char *new_arg);

void		initialize_main(t_minishell **minishell, char **envp);

/* ************************************************************************** */
/*                        Command Executer Helpers                          */
/* ************************************************************************** */
void		ce_setup_child_process(t_minishell *minishell, t_command *cmd,
				int prev_pipe_in, int pipe_fd[2]);
int			ce_init_pipe(t_command *cmd,
				int pipe_fd[2]);
void		ce_process_parent(t_command **cmd,
				int *prev_pipe_in, int pipe_fd[2]);
void		ce_wait_for_all_children(t_minishell *minishell,
				pid_t last_pid);

// command_executer_redirect.c
int			ce_process_command_in_pipeline(t_minishell *minishell,
				t_command **cmd, int *prev_pipe_in, pid_t *last_pid);
int			ce_handle_single_output_redirection(t_token *token);
int			ce_check_output_redirections(t_token *tokens);
int			ce_check_input_redirections(t_token *tokens);

/* ************************************************************************** */
/*                           Command Executer                               */
/* ************************************************************************** */
int			command_executer(t_minishell *minishell);

#endif