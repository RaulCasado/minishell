#include "minishell.h"

t_command	*create_command(t_token *tokens)
{
	t_command	*command;
	t_token		*token;
	int			i;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = malloc(sizeof(char *) * (token_count(tokens) + 1));
	if (!command->args)
	{
		free(command);
		return (NULL);
	}
	i = 0;
	token = tokens;
	while (token)
	{
		command->args[i] = ft_strdup(token->value);
		if (!command->args[i])
		{
			free_command(command);
			return (NULL);
		}
		token = token->next;
		i++;
	}
	command->args[i] = NULL;
	command->pipe_in = STDIN_FILENO;
	command->pipe_out = STDOUT_FILENO;
	command->infile = NULL;
	command->outfile = NULL;
	command->append = 0;
	command->next = NULL;
	return (command);
}