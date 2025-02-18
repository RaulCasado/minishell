
#include "minishell.h"

/*
	write(p[1], msg1, MSGSIZE);
	execve("/bin/echo", argv, NULL);
*/
char	echo(t_command *command)
{
	int pipe[2];
	char	*argv[] = {"echo", "-n", "hola que tal", NULL};

	char	*msg;
	char	leap;
	size_t	arg_len = ft_strlen(argv[2]);

	leap = !ft_strnstr(argv[1], "-n", ft_strlen(argv[1]));
	if (leap)
		msg = ft_calloc((arg_len + 1 + 1), sizeof(char));
	else
		msg = ft_calloc((arg_len + 1), sizeof(char));

	if (!msg)
		return (1);
	if (leap)
		msg[arg_len + 1] = '\n';
	ft_strlcpy(msg, argv[2], arg_len);

	/* if (0) // There's a redir
		write(pipe[1], msg, BUFFER_SIZE);
	else */
	ft_putstr_fd(msg, pipe[1]);
	return (0);
}