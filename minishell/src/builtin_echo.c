
#include "minishell.h"

/*
	/!\ /!\ Testing Grounds /!\ /!\

	write(p[1], msg1, MSGSIZE);
	execve("/bin/echo", argv, NULL);
*/
char	echo(t_command *command)
{
	ft_putstr_fd(command->args[2], command->pipe_out);
	if (!ft_strnstr(command->args[1], "-n", ft_strlen(command->args[1])))
		ft_putstr_fd('\n', command->pipe_out);
	return (0);
}

char	echo2(t_command *command)
{
	int pipe[2];
	char	*argv = command->args; //[] {"echo", "-n", "hola que tal", NULL};

	char	*msg;
	char	new_line;
	size_t	arg_len = ft_strlen(argv[2]);

	new_line = !ft_strnstr(argv[1], "-n", ft_strlen(argv[1]));
	if (new_line)
		msg = ft_calloc((arg_len + 1 + 1), sizeof(char));
	else
		msg = ft_calloc((arg_len + 1), sizeof(char));

	if (!msg)
		return (1);
	if (new_line)
		msg[arg_len + 1] = '\n';
	ft_strlcpy(msg, argv[2], arg_len);

	/* if (0) // There's a redir
		write(pipe[1], msg, BUFFER_SIZE);
	else */
	ft_putstr_fd(msg, command->pipe_in);
	return (0);
}
