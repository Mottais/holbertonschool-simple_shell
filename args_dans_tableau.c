#include "main.h"

/**
 * args_dans_tableau - placer les args (mots) d'une string dans un tableau
 *
 * @user_input: chaine de caractère à traiter
 * Return: tableau d'argument (mots) de l'input
 */
char **args_dans_tableau(char *user_input)
{
	const char *chaineEntree = user_input;
	int nbMots = 0, i;
	char **mots;
	char *copieChaine = strdup(chaineEntree);
	char *jeton = strtok(copieChaine, " \n\t");

	while (jeton != NULL) /* compte ne nombre de mot */
	{
		nbMots++;
		jeton = strtok(NULL, " \n\t");
	}

	mots = malloc((nbMots + 1) * sizeof(char *)); /* dimensionne tableau */

	free(copieChaine);
	copieChaine = strdup(chaineEntree); /* réinitialise la copieChaine */

	jeton = strtok(copieChaine, " ");

	i = 0;
	while (jeton != NULL) /* place les mots dans le tableau */
	{
		mots[i] = strdup(jeton);
		i++;
		jeton = strtok(NULL, " ");
	}
	mots[i] = NULL; /* ajoute un mot NULL nécessaire pour execve */

	free(copieChaine);

	return (mots);
}
