#include "minishell.h"

static char **add_arg(char **args, char *new_arg)
{
    int count = 0;
    while (args && args[count])
        count++;

    char **new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return (NULL);

    for (int i = 0; i < count; i++)
        new_args[i] = args[i];

    new_args[count] = strdup(new_arg);
    new_args[count + 1] = NULL;

    free(args);
    return new_args;
}

static t_command *create_command(char **args, char *infile, char *outfile, int append, int pipe_in, int pipe_out, t_command *next)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = args;
    cmd->infile = infile;
    cmd->outfile = outfile;
    cmd->append = append;
    cmd->pipe_in = pipe_in;
    cmd->pipe_out = pipe_out;
    cmd->next = next;
    return (cmd);
}

t_command *parse_tokens(t_token *tokens)
{
    t_command *cmd_list = NULL;
    t_command *current_cmd = NULL;
    char **args = NULL;
    char *infile = NULL;
    char *outfile = NULL;
    int append = 0, pipe_in = 0, pipe_out = 0;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD) // Guardamos argumentos normales
            args = add_arg(args, tokens->value);
        else if (tokens->type == TOKEN_REDIR_IN) // "<"
        {
            if (tokens->next)
            {
                infile = tokens->next->value;
                tokens = tokens->next; // Saltamos el nombre del archivo
            }
        }
        else if (tokens->type == TOKEN_REDIR_OUT) // ">"
        {
            if (tokens->next)
            {
                outfile = tokens->next->value;
                append = 1;
                tokens = tokens->next; // Saltamos el nombre del archivo
            }
        }
        else if (tokens->type == TOKEN_REDIR_APPEND) // ">>"
        {
            if (tokens->next)
            {
                outfile = tokens->next->value;
                append = 2;
                tokens = tokens->next; // Saltamos el nombre del archivo
            }
        }
        else if (tokens->type == TOKEN_PIPE) // "|"
        {
            pipe_out = 1;
            t_command *new_cmd = create_command(args, infile, outfile, append, pipe_in, pipe_out, NULL);
            
            if (!cmd_list)
                cmd_list = new_cmd;
            else
                current_cmd->next = new_cmd;

            current_cmd = new_cmd;
            args = NULL;
            infile = NULL;
            outfile = NULL;
            append = 0;
            pipe_in = 1;
            pipe_out = 0;
        }
        tokens = tokens->next;
    }

    t_command *last_cmd = create_command(args, infile, outfile, append, pipe_in, pipe_out, NULL);
    if (!cmd_list)
        cmd_list = last_cmd;
    else
        current_cmd->next = last_cmd;

    return (cmd_list);
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