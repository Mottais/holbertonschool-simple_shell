#ifndef __MAIN_H__
#define __MAIN_H__


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 256

char **args_dans_tableau(char *user_input);
int execute_command(int cmd_trouv, char *args[], char **env,
			int count_cmd, char *av_0);
char *_getenv(const char *name, char **env);
int _which(char **args, char **env);
void libererMemoireTab(char **args);

#endif /*HEADER_H*/

