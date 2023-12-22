#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 256

// Fonction pour diviser la ligne en commandes et arguments
void parse_input(char *input, char *args[]) {
    char *token = strtok(input, " \n\t");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n\t");
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

char *_which(const char *executable_name, char **env)
{
//    char *path = _getenv("PATH", env);
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char full_path[256];
    struct stat file_stat;

    char *jeton = strtok(path_copy, ":");

    while (jeton != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", jeton, executable_name);

        if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
        {
            free(path_copy);
            return strdup(full_path);
        }

        jeton = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main(int ac, char **av, char **env) 
{
    char *buffer = NULL, *full_path;
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
        
printf("-->%s\n",args[0]);
        full_path = _which(args[0], env); /* teste si args[0] existe dans environnement */
        if (full_path)
        {
            printf("full_path-->%s\n",full_path);
            free(args[0]);
            args[0] = strdup(full_path); /* 1er element (commande) complèté avec le chemin */
        }
printf("%s\n",args[0]);

        // si commande = "exit"
        if (strcmp(args[0], "exit") == 0)
        {
            status = 1;
            break;
        }

        // Appeler la fonction pour créer un processus enfant et exécuter la commande
        //status = execute_command(args, env);

        if (isatty(STDIN_FILENO))
            printf("$ "); /* affiche le prompt */
    }

    free(buffer);
    return (status);
}
