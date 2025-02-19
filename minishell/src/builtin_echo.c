
#include "minishell.h"

/*
	/!\ /!\ Testing Grounds /!\ /!\

	write(p[1], msg1, MSGSIZE);
	execve("/bin/echo", argv, NULL);
*/

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

static void	write_echo(char **args, size_t i, char newline, char fd)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], fd);
		i++;
		if (args[i])
			ft_putchar_fd(' ', fd);
	}
	if (newline)
		ft_putchar_fd('\n', fd);
}

char	echo(t_command *command)
{
	size_t	i;
	char	**args;
	char	newline;
	char	fd;

	i = 1;
	args = command->args;
	newline = 1;
	while (is_flag('n', args[i])) // args[i] == "-n"
	{
		newline = 0;
		i++;
	}
	if (command->pipe_out == 0)
		fd = 1;
	/* else
		fd = xd; */
	write_echo(args, i, newline, fd);
	return (0);
}
