#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 256

// Fonction pour diviser la ligne en commandes et arguments
void parse_input(char *input, char *args[]) {
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL;  // La dernière entrée doit être NULL
}

// Fonction pour créer un processus enfant et exécuter la commande
int execute_command(char *args[],char **env) {
    // Fork pour créer un processus enfant
    pid_t pid = fork();
    int status;
    
    if (pid == -1) {
        perror("Erreur lors du fork\n");
        return(EXIT_FAILURE);
    }

    if (pid == 0) 
    {
        if (execve(args[0], args, env) == -1) 
        {        
            perror("execve");
            exit(EXIT_FAILURE);
        }

    } 
    else 
    {
        wait(&status);
    }
    return(status);
}


int main(int ac, char **av, char **env) 
{
    char *buffer = NULL;
    size_t bufsize = 0;
    int status = 0;
    (void)ac, (void)av;

    if (isatty(STDIN_FILENO))
        printf("deb $  "); /* affiche le prompt */

    while (getline(&buffer, &bufsize, stdin) != -1) {
        size_t length = strlen(buffer);

        // Supprimer le caractère de saut de ligne à la fin de chaque ligne
        if (length > 0 && buffer[length - 1] == '\n') {
           buffer[length - 1] = '\0';
        }

        // Diviser la ligne en commandes et arguments
        char *args[MAX_ARGS];
        parse_input(buffer, args);

        // si commande = "exit"
        if (strcmp(args[0], "exit\0") == 0)
        {
            status = 1;
            break;
        }

        // Appeler la fonction pour créer un processus enfant et exécuter la commande
        status = execute_command(args, env);

        if (isatty(STDIN_FILENO))
            printf("$ "); /* affiche le prompt */
    }

    free(buffer);
    return (status);
}
