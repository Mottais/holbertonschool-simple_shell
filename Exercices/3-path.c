#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Récupérer la valeur de la variable d'environnement PATH
    char *path_variable = getenv("PATH");

    // Vérifier si la variable d'environnement existe
    if (path_variable != NULL) {
        // Copier la valeur dans une chaîne modifiable
        char *path_copy = strdup(path_variable);

        // Séparer les répertoires en utilisant le délimiteur ":"
        char *token = strtok(path_copy, ":");

        // Afficher chaque répertoire
        while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, ":");
        }

        // Libérer la mémoire allouée
        free(path_copy);
    } else {
        // La variable d'environnement PATH n'est pas définie
        printf("La variable d'environnement PATH n'est pas définie.\n");
    }

    return 0;
}

