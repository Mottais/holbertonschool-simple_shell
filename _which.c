#include "main.h"
/**
 * _which - fonction qui cherche commande
 *
 * @env: - tableau des varaibles d'environnement
 * @args: - tableau d'argument où args[0] est la commande recherchée
 * Return: 1 si commande trouvée, 0 sinon
 */
int _which(char **args, char **env)
{
        char *path, *path_copy, *jeton;
        char full_path[MAX_ARGS];
        struct stat file_stat;

        /* si arg[0] (commande) est un éxé (full_path/file) */
        if ((stat(args[0], &file_stat) == 0) && S_ISREG(file_stat.st_mode))
                return (1); /* un éxé a été trouvé */

        path = _getenv("PATH", env); /* reccup variable PATH de l'environnement */
                                    /* qui est une de chemins séparés par des ':'*/

        path_copy = strdup(path); /* duplication path  accéssible en écriture */

        jeton = strtok(path_copy, ":");
        /* jeton pointe sur debut de path_copy */
        /* et remplace le 1er caractère ":" par le caract de fin : '\0' */

        while (jeton != NULL) /* jusqu'à la fin de path_copy */
        {
                /* full_path = concatenation de jeton + '/' + arg[0] (commande) */
                snprintf(full_path, sizeof(full_path), "%s/%s", jeton, args[0]);

                /* si full_path est un éxé */
                if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
                {

                        free(path_copy);
                        free(args[0]);
                        args[0] = strdup(full_path); /* modifie arg[0] avec full_path */
                        return (1); /* un éxé a été trouvé */
                }

                jeton = strtok(NULL, ":");
                /* jeton pointe sur le path suiant */
                /* et remplace le prochain caractère ":" par le caract de fin : '\0' */
        }
        free(path_copy);

        return (0); /* pas d'éxé trouvé */
}
