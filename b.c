
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 256

// Fonction pour diviser la ligne en commandes et arguments
void args_dans_tableau(char *input, char *args[]) {
    char *jeton = strtok(input, " \n\t");
    int i = 0;

    while (jeton != NULL && i < MAX_ARGS - 1) 
    {
        args[i++] = jeton;
        jeton = strtok(NULL, " \n\t");
    }

    args[i] = NULL;  /* Ajout arg NULL pour la fonction execve */
}

// Fonction pour créer un processus enfant et exécuter la commande
int execute_command(char *cmd, char *args[],char **env) 
{
    pid_t pid;
    int status;
    
    pid = fork(); /* création processus enfant */
    
    /* si création processus enfant echoue */
    if (pid == -1)  
    {
        perror("Erreur lors du fork\n");
        return(EXIT_FAILURE);
    }
    
    /* processus enfant */
    if (pid == 0)
    {   /* Execute la commande avec les arguments puis si réeussit,ferme le processus enfant */
        if (execve(cmd, args, env) == -1)  
        { /* Si Execution echoue imprime message erreur puis ferme le processus enfant */
            perror("execve");  
            exit(EXIT_FAILURE);
        }
    }
    
    /* processus enfant */
    else
        wait(&status); /* Attend fin processus enfant et reccup status processus enfant*/
        
    return(status);
}

char *_getenv(const char *name, char **env)
{
    if (name == NULL)
        return NULL;

    for (; *env != NULL; ++env) 
    {
        if (strncmp(*env, name, strlen(name)) == 0 )//&& (*env)[strlen(name)] == '=') 
        {
            return *env + strlen(name) + 1;
        }
    }

    return NULL;
}
char *_which(const char *cmd, char **env)
{
    char *path, *path_copy;
    char full_path[256];
    struct stat file_stat;
    
    path = _getenv("PATH", env);
    path_copy = strdup(path);

    char *jeton = strtok(path_copy, ":");

    while (jeton != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", jeton, cmd);

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
    char *user_input = NULL, *full_path, *args[MAX_ARGS];;
    size_t bufsize = 0, length;
    int status = 0;
    (void)ac, (void)av;

    if (isatty(STDIN_FILENO))
        printf("$  "); /* affiche le prompt */

    while (getline(&user_input, &bufsize, stdin) != -1) /* attend saisie utilisateur */
    {
        length = strlen(user_input);

        /* remplace le caractère de saut de ligne par caractere null */
//        if (length > 0 && user_input[length - 1] == '\n')
           user_input[length - 1] = '\0';
           
if (length > 1)
{

        /* cré un tableau avec chaque mots de la saisie utilisateur*/
        args_dans_tableau(user_input, args);

        // si commande = "exit"
        if (strcmp(args[0], "exit") == 0)
        {
            status = 1;
            break;
        }

        full_path = _which(args[0], env); /* teste si args[0] existe dans environnement */
        if (full_path)
        {
            status = execute_command(full_path, args, env);
            free(full_path);
        }
        else
        status = execute_command(args[0], args, env);
}
        if (isatty(STDIN_FILENO))
            printf("$ "); /* affiche le prompt */
    }

    free(user_input);
    return (status);
}
