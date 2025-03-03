#include <stdio.h>    // For printf()
#include <stdlib.h>   // For exit()
#include <unistd.h>   // For pipe(), read(), write(), fork(), execve()
#include <string.h>   // For strlen()
#include <sys/types.h> // For pid_t
#include <sys/wait.h>
#define MSGSIZE 16 

void child_process(int p[2]) {
    // Redirect stdout to the pipe
    close(p[0]); // Close unused read end
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);

    // Execute "echo -n hola que tal"
    char	*argv[] = {"/bin/echo", "-n", "hola que tal", NULL};
    execve("/bin/echo", argv, NULL);

    // If execve fails
    perror("execve");
    exit(1);
}

void parent_process(int p[2], pid_t pid, int show_output) {
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

int main() { 
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
        child_process(p);
    } else { // Parent process
        parent_process(p, pid, show_output);
    }

    return 0; 
}