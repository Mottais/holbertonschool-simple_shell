#include "main.h"
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
