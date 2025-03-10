#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static int	open_output_file(char *file, int append)
{
	int	fd;

	if (append == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (append == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		return (1);
	if (fd == -1)
	{
		perror("Minishell: open");
		return (1);
	}
	return (fd);
}

static char	handle_outfile(t_command *cmd)
{
	int	fd;

	if (cmd->outfile[0] == '\0' || (unsigned char)cmd->outfile[0] > 127)
		return (1);
	fd = open_output_file(cmd->outfile, cmd->append);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
}

static char	handle_infile(t_command *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY); // infile < echo <--- blow up
	if (fd == -1)
	{
		perror("Minishell: open");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
}

int	handle_redirections(t_command *cmd)
{
	int	fd;

	if (cmd->outfile)
	{
		if (handle_outfile(cmd))
			return (-1);
	}
	if (cmd->infile)
	{
		if (handle_infile(cmd))
			return (-1);
	}
	// Minishell> echo hola > outfile
	// Minishell> cat outfile
	return (0);
}
