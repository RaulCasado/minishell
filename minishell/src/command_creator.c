#include "minishell.h"

static char **add_arg(char **args, char *new_arg)
{
	int count = 0;
	int i = 0;
	while (args && args[count])
		count++;

	char **new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);

	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}

	new_args[count] = strdup(new_arg);
	new_args[count + 1] = NULL;

	free(args);
	return new_args;
}

// create a struct to not pass too many arguments
t_command *create_command(t_command_info *ci, t_command *next)
{
    t_command    *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = ci->args;
    cmd->infile = ci->infile;
    cmd->outfile = ci->outfile;
    cmd->append = ci->append;
    cmd->pipe_in = ci->pipe_in;
    cmd->pipe_out = ci->pipe_out;
    cmd->next = next;
    return (cmd);
}

static void handle_redirection(t_token **t, t_command_info *ci, int type)
{
    if (!(*t)->next)
        return;
    *t = (*t)->next;
    if (type == TOKEN_REDIR_IN)
        ci->infile = (*t)->value;
    else if (type == TOKEN_REDIR_OUT)
        ci->outfile = (*t)->value, ci->append = 1;
    else if (type == TOKEN_REDIR_APPEND)
        ci->outfile = (*t)->value, ci->append = 2;
}

static void handle_pipe(t_command **cl, t_command_info *ci, t_command **curr)
{
	ci->pipe_out = 1;
	if (*cl)
		*curr = create_command(ci, *curr);
	else
	{
		*curr = create_command(ci, NULL);
		*cl = *curr;
	}
	ci->args = NULL;
	ci->infile = NULL;
	ci->outfile = NULL;
	ci->append = 0;
	ci->pipe_in = 1;
	ci->pipe_out = 0;
}

static void handle_word(t_command_info *ci, char *value)
{
    ci->args = add_arg(ci->args, value);
}

t_command *parse_tokens(t_token *t)
{
    t_command        *cl = NULL;
    t_command        *curr = NULL;
    t_command_info   ci = {0};

    while (t)
    {
        if (t->type == TOKEN_WORD)
            handle_word(&ci, t->value);
        else if (t->type >= TOKEN_REDIR_IN && t->type <= TOKEN_REDIR_APPEND)
            handle_redirection(&t, &ci, t->type);
        else if (t->type == TOKEN_PIPE)
            handle_pipe(&cl, &ci, &curr);
        t = t->next;
    }
    if (cl)
        curr->next = create_command(&ci, NULL);
    else
        cl = create_command(&ci, NULL);
    return (cl);
}

void print_commands(t_command *commands)
{
	while (commands)
	{
		for (int i = 0; commands->args && commands->args[i]; i++)
			printf("args[%d]: %s\n", i, commands->args[i]);
		printf("infile: %s\n", commands->infile);
		printf("outfile: %s\n", commands->outfile);
		printf("append: %d\n", commands->append);
		printf("pipe_in: %d\n", commands->pipe_in);
		printf("pipe_out: %d\n", commands->pipe_out);
		printf("\n");
		commands = commands->next;
	}
}
