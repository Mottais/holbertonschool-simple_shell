#include "main.h"
/**
 * execute_command - Fonction pour exécuter la commande
 *
 * @cmd_trouv: =1 si args[0] est un éxé, 0 sinon
 * @args: - tableau des arguements
 * @env: environement
 * @count_cmd: compteur du nombre de fois qu'une commande a été exécutée
 * @av_0: - nom de la fonction, exemple : ./hsh
 * Return: status
 */
/* Fonction pour créer un processus enfant et exécuter la commande */
int execute_command(int cmd_trouv, char *args[], char **env,
					int count_cmd, char *av_0)
{
	pid_t pid;
	int status = 0;

	if (cmd_trouv == 0) /* si aucun éxé trouvé */
	{
		/* msg err */
		fprintf(stderr, "%s: %d: %s: not found\n", av_0, count_cmd, args[0]);
		return (127); /* retour code err */
	}
	else
	{
		pid = fork(); /* création processus enfant */

		/* si création processus enfant echoue */
		if (pid == -1)
		{
			perror("Erreur lors du fork\n");
			return (EXIT_FAILURE);
		}
		/* processus enfant */
		if (pid == 0)
		{
			/* Exec cmd avec les arguments puis si réussit, ferme le processus */
			if (execve(args[0], args, env) == -1)
			{
				/* Si Exec échoue, imprime msg err et ferme processus enfant */
				fprintf(stderr, "%s: %d: %s: not found\n", av_0, count_cmd, args[0]);
				exit(127);
			}
		}
		/* processus parent */
		else if (pid > 0)
		{
			wait(&status);/* Attend fin process enfant et reccup status son processus */
			if ((WEXITSTATUS(status)) != 0)/* si status de processus enfant non null */
				status = (WEXITSTATUS(status)); /* réccup status  processus enfant */
			return (status);
		}
	}
	return (0);
}
