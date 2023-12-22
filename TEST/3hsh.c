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
void execute_command(char *args[]) {
    // Fork pour créer un processus enfant
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Code du processus enfant

        // Utiliser execvp pour exécuter la commande
        if (execvp(args[0], args) == -1) {
            perror("Erreur lors de l'exécution de la commande");
            exit(EXIT_FAILURE);
        }
    } else {
        // Code du processus parent

        // Attendre la terminaison du processus enfant
        int status;
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("Erreur lors de l'attente du processus enfant");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    char *buffer = NULL;
    size_t bufsize = 0;

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

        // Appeler la fonction pour créer un processus enfant et exécuter la commande
        execute_command(args);

        if (isatty(STDIN_FILENO))
            printf("$ "); /* affiche le prompt */
    }

    free(buffer);
    return 0;
}

