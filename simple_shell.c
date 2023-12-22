#include "main.h"

/**
 * main - function that prints '$' to standard output
 *
 * @ac: -
 * @av: -
 * @env: -
 * Return: status
 */
int main(int ac, char **av, char **env)
{
	char *user_input = NULL, **args;
	size_t bufsize = 0;
	ssize_t  length;
	int  count_cmd = 0, cmd_trouv, status = 0;
	(void)ac, (void)av;

	if (isatty(STDIN_FILENO)) /* si mode interractif */
		printf("$ "); /* affiche le prompt */

	/* input utilisateur : saisie clavier si ineterractif sinon input standard */
	while ((length = getline(&user_input, &bufsize, stdin)) != -1)
	{
		user_input[length - 1] = '\0'; /* remplace dernier caractère par \0*/

		if (length > 1)
		{
			count_cmd++; /*compte nb de fois qu'une commande est éxé (pour msg err) */
			args = args_dans_tableau(user_input);/*créer tableau avec args de input*/

			if (strcmp(args[0], "exit") == 0)   /*si commande (args[0])= "exit" */
			{
				libererMemoireTab(args); /* Libérer la mémoire allouée au tab args  */
				break;  /*quitte la boucle pour quitter le programme */
			}
			/*teste si args[0] existe dans env et complete arg[0] avec path si besoin*/
			cmd_trouv = _which(args, env);

			/* Exceute commande et retourn code erreur le cas echeant*/
			status = execute_command(cmd_trouv, args, env, count_cmd, av[0]);

			/* Libérer la mémoire allouée a args  */
			libererMemoireTab(args);
		}

		if (isatty(STDIN_FILENO)) /* si mode interractif */
			printf("$ "); /* affiche le prompt */

	} /*fin while*/
	free(user_input); /* libere la mémore issue de la fonction getline */
	return (status);
}
