#include <stdio.h>
#include <stdlib.h>

int main() {
    char *user_input = NULL;
    size_t input_size = 0;


        printf("$ ");

        // Utiliser getline pour lire la saisie de l'utilisateur
        ssize_t bytes_read = getline(&user_input, &input_size, stdin);

        // Vérifier si la lecture a échoué (fin de fichier ou erreur)
        if (bytes_read == -1) 
            perror("Erreur de lecture");


        // Afficher la commande sur la ligne suivante
        printf("%s", user_input);
        printf("%lu\n", bytes_read);
        printf("%lu", input_size);

    // Libérer la mémoire allouée par getline
    free(user_input);

    return 0;
}
