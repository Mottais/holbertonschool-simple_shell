#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 256



/**
 * args_dans_tableau - function that prints '$' to standard output
 *
 * @user_input: -
 * Return: tableau d'argument de l'input
 */
/* Fonction pour diviser la ligne en commandes et arguments*/
char **args_dans_tableau(char *user_input)
{
	const char *chaineEntree = user_input;
	int nbMots = 0, i;
	char **mots;
	char *copieChaine = strdup(chaineEntree);
	char *jeton = strtok(copieChaine, " \n\t");

	while (jeton != NULL)
	{
		nbMots++;
		jeton = strtok(NULL, " \n\t");
	}

	mots = malloc((nbMots + 1) * sizeof(char *));

	free(copieChaine);
	copieChaine = strdup(chaineEntree);

	jeton = strtok(copieChaine, " ");

	i = 0;
	while (jeton != NULL)
	{
		mots[i] = strdup(jeton);
		i++;
		jeton = strtok(NULL, " ");
	}
	mots[i] = NULL;

	free(copieChaine);

	return (mots);
}


/**
 * execute_command - function that prints '$' to standard output
 *
 * @cmd_trouv: =1 si args[0] est un éxé, 0 sinon
 * @args: - tableau des arguements
 * @env: environement
 * @count_cmd: -
 * @av_0: - nom de la fonctio, exemple : ./hsh
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
			/* Exec cmd avec les arguments puis si réeussit,ferme le processus */
			if (execve(args[0], args, env) == -1)
			{
				/*Si Exec echoue, imprime msg err et ferme processus enfant */
				fprintf(stderr, "%s: %d: %s: not found\n", av_0, count_cmd, args[0]);
				exit(127);
			}
		}
		/* processus parent */
		else if (pid > 0)
		{
			wait(&status); /*Attend fin process enfant et reccup status son processus */
			if ((WEXITSTATUS(status)) != 0)  /*si status de processus enfant non null */
				status = (WEXITSTATUS(status)); /*réccup status  processus enfant */
			return (status);
		}
	}
	return (0);
}

/**
 * _getenv - function that prints '$' to standard output
 *
 * @name: -
 * @env: -
 * Return: NULL
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

/**
 * _which - function that prints '$' to standard output
 *
 * @env: -
 * @args: -
 * Return: NULL
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

/**
 * libererMemoireTab - libere la mémoire d'un tableau
 *
 * @args: -
 */
void libererMemoireTab(char **args)
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	free(args[i]);

	free(args);
}

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
