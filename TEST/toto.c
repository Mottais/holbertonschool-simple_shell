#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char *buffer = NULL;
    size_t bufsize = 0;

	if (isatty(STDIN_FILENO))
	printf("deb $  "); /* affiche le prompt */

    // Lecture de chaque ligne depuis l'entrée standard
	while (getline(&buffer, &bufsize, stdin) != -1)
	{
        	// Supprimer le caractère de saut de ligne à la fin de chaque ligne
        	size_t length = strlen(buffer);
        	if (length > 0 && buffer[length - 1] == '\n') 
		{
        		buffer[length - 1] = '\0';
			printf("Lecture de chaque ligne depuis l'entrée standard\n");
        	}

        	// Exécuter la commande
        	if (system(buffer) == -1) 
		{
			printf(" Exécuter la commande\n");
			perror("Erreur lors de l'exécution de la commande");
			free(buffer);
			exit(EXIT_FAILURE);
        	}
		if (isatty(STDIN_FILENO))
        		printf("$ "); /* affiche le prompt */
	}

    free(buffer);
    return 0;
}

