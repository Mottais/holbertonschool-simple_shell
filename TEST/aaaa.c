#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
void _commande_is_exit()
{
    printf("Commande 'exit' à traiter : Yacine ?\n");
    exit(0);
}

void free_memory(char ***mots, char **user_input)    
{
    int i;

    if (*mots != NULL) {
        for (i = 0; (*mots)[i]; i++)
            free((*mots)[i]);

        free(*mots);
    }

    if (*user_input)
    {
        free(*user_input);
        *user_input = NULL;
    }
}

void exe_cmd(char **cmd, char **env)
{
    pid_t pid = fork();

    if (pid == 0) {
        if (execve(cmd[0], cmd, env) == -1) {        
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else {
        int status;
        wait(&status);

    }
}

char **mots_dans_tableau(char *user_input)
{
        const char *chaineEntree = user_input;       
        int nbMots = 0, i;
        char *copieChaine = strdup(chaineEntree);    
        char *jeton, **mots;

        jeton = strtok(copieChaine, " \n\t");
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
    while (jeton != NULL) {
        mots[i] = strdup(jeton);
        i++;
        jeton = strtok(NULL, " ");
    }
    mots[i] = NULL;

    free(copieChaine);

    return mots;
}

char *_getenv(const char *name, char **env)
{
    if (name == NULL)
        return NULL;

    for (; *env != NULL; ++env) {
        if (strncmp(*env, name, strlen(name)) == 0 && (*env)[strlen(name)] == '=') {
            return *env + strlen(name) + 1;
        }
    }

    return NULL;
}

char *_which(const char *executable_name, char **env)
{
    char *path = _getenv("PATH", env);
    char *path_copy = strdup(path);
    char full_path[256];
    struct stat file_stat;

    char *jeton = strtok(path_copy, ":");

    while (jeton != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", jeton, executable_name);

        if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
        {
            free(path_copy);
            return strdup(full_path);
        }

        jeton = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main(int ac, char **av, char **env)
{
    char *user_input = NULL, **mots, *full_path, *executable_name;
    size_t input_size = 0;
    int bytes_read = 0;

    (void)ac, (void)av;
    while (1)
    {

        printf("$ "); /* affiche le prompt */

        bytes_read = getline(&user_input, &input_size, stdin); /* attend saisie utilisateur */

        if (bytes_read == -1) /* si erreur getline */
        {
            printf("\n");
            if (user_input)
                free_memory(&mots, &user_input);
            return 0;
        }

        user_input[bytes_read - 1] = '\0'; /* force le dernier caractere à null */

        mots = mots_dans_tableau(user_input); /* créé un tableau avec chaque mots le la saisie utilisateur*/
        executable_name = mots[0];
/* A finir : gestion exit */
        if (strcmp(executable_name, "exit") == 0)
        {
            free_memory(&mots, &user_input);
            _commande_is_exit();
        }

        full_path = _which(executable_name, env); /* teste si executable_name existe dans environnement */

        if (full_path)
        {
            free(mots[0]);
            mots[0] = strdup(full_path); /* 1er element (commande) complèté avec le chemin */
            exe_cmd(mots, env);
        }
        else if (access(mots[0], X_OK) == 0)
                exe_cmd(mots, env);
        else
        {
                fprintf(stderr, "%s: %s: No such file or directory\n", __FILE__, executable_name);
        }

        free_memory(&mots, &user_input); /* Libérer la mémoire allouée */
    }
printf("free\n");
	free_memory(&mots, &user_input);
    return 0;
}
