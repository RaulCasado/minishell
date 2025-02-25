
#include "minishell.h"
#include <sys/wait.h>

#define MSGSIZE 512 


/*	This file is a prototype to build the Pipex	*/


void child_process(int p[2], t_command *command)
{
	// Redirect stdout to the pipe
	close(p[0]); // Close unused read end
	dup2(p[1], STDOUT_FILENO);
	close(p[1]);

	char	*cmd = ft_strjoin("/bin/", command->args[0]);

	// Execute "echo -n hola que tal"
	char	*argv[] = {cmd, command->args[1], NULL};
	execve(cmd, argv, NULL);

	// If execve fails
	perror("execve");
	exit(1);
}

void parent_process(int p[2], pid_t pid, int show_output)
{
	char	inbuf[MSGSIZE];
	int		i;

	// Close unused write end
	close(p[1]);

	// Read from the pipe
	while (read(p[0], inbuf, MSGSIZE) > 0) {
		if (show_output) {
			printf("%s", inbuf);
		}
	}

	// Close read end
	close(p[0]);

	// Wait for child process to finish
	waitpid(pid, NULL, 0);
}

static void	initialize_memory(int **pipedes, char **buffer)
{
	*pipedes = malloc(sizeof(int) * 3);
	if (!*pipedes)
		exit(1);
	if (pipe(*pipedes) < 0)
	{
		free(*pipedes);
		exit(1);
	}
	ft_putendl_fd("Pipeada", 1);
	*buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (!*buffer)
	{
		free(*pipedes);
		exit(1);
	}
	*pipedes[2] = '\0';
	ft_bzero(*buffer, BUFFER_SIZE);
}

char	command_process(t_minishell *minishell, t_command *command)
{
	int		p[2]; 
	pid_t	pid;
	int		show_output = 1; // Cambia a 0 para no mostrar la salida en pantalla

	// Create a pipe and check for errors
	if (pipe(p) < 0) { 
		perror("pipe"); 
		exit(1); 
	} 

	// Fork a child process
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}

	if (pid == 0) { // Child process
		child_process(p, command);
	} else { // Parent process
		parent_process(p, pid, show_output);
	}

	return 0; 
}
