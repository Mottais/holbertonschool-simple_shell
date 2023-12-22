#include "main.h"

/**
 * _getenv - fonction qui recup une variable d'environnement
 *
 * @name: nom de la variable a récup
 * @env: tableau des varaibles d'environnement
 * Return: valeur de la variable (name); NULL si pa trouvée
 */
char *_getenv(const char *name, char **env)
{
	if (name == NULL)
		return (NULL);

	for (; *env != NULL; ++env) /* boucle dans le tableau des variablesq d'env */
	{
		/* si début = valeur cherchée (name) + suivi de carac '=' */
		if (strncmp(*env, name, strlen(name)) == 0 && (*env)[strlen(name)] == '=')
		{
			return (*env + strlen(name) + 1); /* revoi variable name apres le '=' */
		}
	}

	return (NULL); /* si pas trouvée la variable name */
}
