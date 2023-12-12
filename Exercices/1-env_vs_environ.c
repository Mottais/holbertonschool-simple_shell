#include <stdio.h>

extern char **environ;  // Déclare la variable externe environ

int main(int argc, char *argv[], char *env[]) {
    printf("Adresse de env (paramètre main) : %p\n", env);
    printf("Adresse de environ (variable globale) : %p\n", environ);

    // Vérifie si les adresses sont les mêmes
    if (env == environ) {
        printf("env et environ ont la même adresse.\n");
    } else {
        printf("env et environ ont des adresses différentes.\n");
    }

    return 0;
}

