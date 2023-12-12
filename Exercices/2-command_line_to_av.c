#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char* chaineEntree = " Bonjour,  comment ça va   ?";
    int nbMots = 0;  // Initialisez la variable à zéro

    // Copier la chaîne d'entrée pour éviter de la modifier
    char* copieChaine = strdup(chaineEntree);

    // Compter le nombre de mots
    char* jeton = strtok(copieChaine, " ");
    while (jeton != NULL) {
        nbMots++;
        jeton = strtok(NULL, " ");
    }

    // Allouer de la mémoire pour le tableau de chaînes
    char** mots = malloc(nbMots * sizeof(char*));

    // Réinitialiser la copie de la chaîne et remplir le tableau avec les mots
    int i = 0;
    
    free(copieChaine);
    copieChaine = strdup(chaineEntree);
    
    jeton = strtok(copieChaine, " ");
    while (jeton != NULL) {
        mots[i] = strdup(jeton);
        i++;
        jeton = strtok(NULL, " ");
    }

    // Afficher les mots
    printf("Nombre de mots : %d\n", nbMots);
    for (i = 0; i < nbMots; i++) {
        printf("Mot %d : %s\n", i + 1, mots[i]);
    }

    // Libérer la mémoire allouée
    for (i = 0; i < nbMots; i++) {
        free(mots[i]);
    }
    free(mots);

    // Libérer la copie temporaire de la chaîne
    free(copieChaine);

    return 0;
}
