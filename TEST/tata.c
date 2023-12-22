#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main() {
    char *buffer = NULL;
    size_t bufsize = 0;
if (isatty(STDIN_FILENO))
printf("deb $  "); /* affiche le prompt */	
    // Lecture de chaque ligne depuis l'entrée standard
    while (getline(&buffer, &bufsize, stdin) != -1) {
        // Supprimer le caractère de saut de ligne à la fin de chaque ligne
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

        // Fork pour créer un processus enfant
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Erreur lors du fork");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Code du processus enfant

            // Diviser la ligne en commandes et arguments
            char *token = strtok(buffer, " ");
            char *args[256];  // Vous pouvez ajuster la taille selon vos besoins
            int i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;  // La dernière entrée doit être NULL

            // Utiliser execve pour exécuter la commande
            if (execve(args[0], args, NULL) == -1) {
                perror("Erreur lors de l'exécution de la commande");
                free(buffer);
                exit(EXIT_FAILURE);
            }
        } else {
            // Code du processus parent

            // Attendre la terminaison du processus enfant
            int status;
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("Erreur lors de l'attente du processus enfant");
                free(buffer);
                exit(EXIT_FAILURE);
            }
        }
if (isatty(STDIN_FILENO))
printf("$ "); /* affiche le prompt */
    }

    free(buffer);
    return 0;
}

