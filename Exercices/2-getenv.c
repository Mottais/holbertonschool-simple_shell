#include <stdio.h>
#include <string.h>


char *_getenv(const char *name)
{
    extern char **environ;  // Déclare la variable externe copie_environ
    char **copie_environ =environ;
    if (name == NULL)
        return NULL;

    // Itère à travers le tableau copie_environ pour trouver la variable d'environnement spécifiée
    for (; copie_environ != NULL; ++copie_environ) 
        // Vérifie si la variable d'environnement actuelle commence par la variable recherché suivi de '='
        if (strncmp(*copie_environ, name, strlen(name)) == 0 && (*copie_environ)[strlen(name)] == '=')
            return *copie_environ + strlen(name) + 1;
        

    // Retourne NULL si la variable d'environnement n'est pas trouvée
    return NULL;
}

int main(int ac, char **av) 
{
    int i;
    
    if (ac <2)
        printf("saisir au moins un argument. Exemple : PATH");
    
    for (i = 1; i < ac; i++)
    {
    char *valeurVariable = _getenv(av[i]);

    if (valeurVariable != NULL)
        printf("%s= \n %s\n", av[i], valeurVariable);
    else
        printf("%s non trouvé dans l'environnement.\n", av[i]);
    }

    return 0;
}

