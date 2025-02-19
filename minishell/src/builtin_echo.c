
#include "minishell.h"

static char	is_flag(char flag, char *arg)
{
	size_t	i;

	if (ft_strlen(arg) >= 2 && arg[0] == '-')
	{
		i = 1;
		while (arg[i] == flag)
		{
			i++;
			if (!arg[i])
				return (1);
		}
	}
	return (0);
}

char	builtin_echo(t_command *command)
{
	size_t	i;
	char	**args;
	char	newline;
	char	fd;

	i = 1;
	args = command->args;
	newline = 1;
	while (is_flag('n', args[i]))
	{
		newline = 0;
		i++;
	}
	if (command->pipe_out == 0)
		fd = 1;
	/* else
		fd = xd; */
	// write_echo(args, i, newline, fd);
	write_strs(args + i, newline, fd);
	return (0);
}
