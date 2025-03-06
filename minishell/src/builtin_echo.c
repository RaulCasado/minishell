#include "minishell.h"

static char is_flag(char flag, char *arg)
{
    size_t i;

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

int builtin_echo(t_minishell *minishell, t_command *command)
{
	size_t  i;
	char    **args;
	char    newline;
	int     fd;

	i = 1;
	args = command->args;
	newline = 1;

	while (args[i] && is_flag('n', args[i]))
	{
		newline = 0;
		i++;
	}

	fd = STDOUT_FILENO;
	if (write_strs(args + i, newline, 0, fd) == -1)
	{
		perror("minishell: echo");
		return (1);
	}
	return (0);
}
