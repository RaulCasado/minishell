#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char **args, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("Error al crear proceso");
        return;
    }
    if (pid == 0) // Proceso hijo esto habria que mirarlo pq no lo entiendo bien
    {
        execve(args[0], args, envp); 
        perror("execve"); // Solo se ejecuta si execve falla
        exit(127); 
    }
    else // Proceso padre
    {
        waitpid(pid, &status, 0); // Espera a que termine el proceso hijo
        printf("Proceso terminado con código de salida: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    
    // Simula el comando `ls -l -h`
    char *args[] = {"/bin/ls", "-l", "-h", NULL};

    printf("Ejecutando comando: %s %s %s\n", args[0], args[1], args[2]);

    execute_command(args, envp);

    return 0;
}
