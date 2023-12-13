#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Fonction pour rechercher un exécutable dans le chemin d'accès
char *_which(const char *executable_name, char **env) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char full_path[256];
    struct stat file_stat;

    char *token = strtok(path_copy, ":");

    while (token != NULL) {
        
        // Construire le chemin complet
        snprintf(full_path, sizeof(full_path), "%s/%s", token, executable_name);

        // Vérifier si le fichier existe
        if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) 
        {
            free(path_copy);
            return (strdup(full_path));
        }

        // Passer au prochain répertoire dans le chemin d'accès
        token = strtok(NULL, ":");
    }

    
    free(path_copy);
    return (NULL);
}

int main(int ac, char **av, char **env) 
{
    char* full_path;
    const char *executable_name = "ls";
    full_path = _which(executable_name, env);
    if (full_path)
        printf("L'exécutable '%s' a été trouvé à l'emplacement : %s\n", executable_name, full_path);
    else
        printf("L'exécutable '%s' n'a pas été trouvé dans le chemin d'accès.\n", executable_name);

    return 0;
}

