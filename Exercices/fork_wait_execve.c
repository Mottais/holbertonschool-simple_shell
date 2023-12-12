#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
 * main - fork example
 *
 * Return: Always 0.
 */
int main(void)
{
    int i, status = 0;
    pid_t child_pid;
    char *argv[] = {"/bin/ls","-l", "/usr/", NULL};

    
    
    for(i = 0; i < 5; i++)
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            perror("Error:");
            return (1);
        }
        if (child_pid == 0)
        {
            if (execve(argv[0], argv, NULL) == -1)
            perror("Error:");
        }
        wait(&status);
    }

    
    return (0);
}
