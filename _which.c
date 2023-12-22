#include "main.h"

/**
 * _which - fonction qui cherche commande
 *
 * @env: tableau des variables d'environnement
 * @args: tableau d'argument où args[0] est la commande recherchée
 * Return: 1 si commande trouvée, 0 sinon
 */
int _which(char **args, char **env)
{
	char *path, *path_copy, *jeton;
	char full_path[MAX_ARGS];
	struct stat file_stat;

	/* si args[0] (commande) est un exécutable (full_path/file) */
	if ((stat(args[0], &file_stat) == 0) && S_ISREG(file_stat.st_mode))
		return (1); /* un exécutable a été trouvé */

	path = _getenv("PATH", env);/*récupère la variable PATH de l'environnement*/
	/* qui est une liste de chemins séparés par des ':'*/

	path_copy = strdup(path); /* duplication de path accessible en écriture */

	jeton = strtok(path_copy, ":");
	/* jeton pointe sur le début de path_copy */
	/* et remplace le premier caractère ":" par le caractère de fin : '\0' */

	while (jeton != NULL) /* jusqu'à la fin de path_copy */
	{
		/* full_path = concaténation de jeton + '/' + args[0] (commande) */
		snprintf(full_path, sizeof(full_path), "%s/%s", jeton, args[0]);

		/* si full_path est un exécutable */
		if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
		{
			free(path_copy);
			free(args[0]);
			args[0] = strdup(full_path); /* modifie args[0] avec full_path */
			return (1);					 /* un exécutable a été trouvé */
		}

		jeton = strtok(NULL, ":");
		/* jeton pointe sur le chemin suivant */
		/* et remplace le prochain caractère ":" par le caractère de fin : '\0' */
	}
	free(path_copy);

	return (0); /* pas d'exécutable trouvé */
}
