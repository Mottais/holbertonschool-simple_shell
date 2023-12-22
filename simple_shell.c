#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

void print_prompt(void);

int exec_cmd(char *command, char **arguments, char **env, char
**argv, int *count_command);

char **parse_command_line(char *command, char **arguments);
void free_args(char **arguments);
char *get_path(char *command, char **env);

/**
 * exec_cmd - execute a command
 * @command: the command to execute
 * @arguments: array of pointers to arguments
 * @env: environment variables
 * @argv: arguments of the ./simple shell
 * @count_command: counts the number of commands executed
 *
 * Description: create a child to execute the command
 * Return: status
 */
int exec_cmd(char *command, char **arguments, char **env,
char **argv, int *count_command)
{
	pid_t pid;
	int status = 0;
	/*struct stat file_stats;*/

	if (command != NULL)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (-1);
		}
		if (pid == 0)
			execve(command, arguments, env);
		else
		{
			wait(&status);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
				status = 2;
		}
		free(command);
	}
	else
	{
		status = 127;
		fprintf(stderr, "%s: %d: %s: not found\n", argv[0],
		*count_command, arguments[0]);
	}
	return (status);
}
/**
 * free_args - free arguments
 * @arguments: arguments
 *
 * Description: free memory
 * Return: Nothing
 */
void free_args(char **arguments)
{
	int i;

	if (arguments != NULL)
	{
		for (i = 0; arguments[i] != NULL; i++)
			free(arguments[i]);
		free(arguments);
	}
}
/**
 * *get_path - get a path where the command can be found
 * @ev: environment variables
 * @command: the command we are searching for
 *
 * Description: Take as input the command typed by the user and return a
 * string containing the path were the command can be found
 * Return: a pointer to the path or null (failure)
 */
char *get_path(char *command, char **ev)
{
	char *path = NULL, *result = NULL, *token = NULL;
	int i, result_len = 0;
	struct stat file_info;

	if (command[0] == '/' || (command[0] == '.'))
		if (stat(command, &file_info) == 0) /*if the command start with '/' or '.'*/
			return (strdup(command)); /*no need to search for a path*/

	for (i = 0; ev[i]; i++) /*get path from environment*/
		if (ev[i][0] == 'P' && ev[i][1] == 'A' && ev[i][2] == 'T' && ev[i][4] == '=')
			path = strdup(ev[i]);

	if (path != NULL)
	{
		token = strtok(path, "="); /*remove "PATH="*/
		token = strtok(NULL, ":");
		while (token != NULL) /*combine each path with the command*/
		{
			result_len = strlen(token) + strlen(command) + 2;
			result = malloc(result_len);
			for (i = 0; i < result_len; i++)
				result[i] = 0;

			strcat(result, token);
			strcat(result, "/");
			strcat(result, command);
			if (stat(result, &file_info) == 0) /*test if the command exist here*/
			{
				free(path);
				return (result);
			}
			free(result);
			result = NULL;
			token = strtok(NULL, ":");
		}
	}
	free(path);
	return (0);
}
/**
 * **parse_command_line - function to get the executable
 * @command: pointer to the command line
 * @arguments: array of pointers to arguments
 *
 * Description: cut the command line into arguments and make an array with it
 * Return: NOTHING
 */
char **parse_command_line(char *command, char **arguments)
{
	char *line = strdup(command), *arg;
	int i = 0, count_args = 0;

	arg = strtok(line, " \n");
	while (arg != NULL) /* counts the number of arguments for malloc*/
	{
		arg = strtok(NULL, " \n");
		count_args++;
	} /* finished counting*/

	free(line);
	if (count_args == 0)
		return (0);

	arguments = realloc(arguments, sizeof(char *) * (count_args + 1));
	if (arguments == NULL)
		exit(EXIT_FAILURE);

	arguments[count_args] = NULL;
	line = strdup(command); /*go back to start*/

	/* put arguments inside array arguments*/
	arg = strtok(line, " \n");
	while (arg != NULL)
	{
		arguments[i] = strdup(arg);
		arg = strtok(NULL, " \n");
		i++;
	}
	free(line);
	return (arguments);
}

/**
 * main - function simple shell
 * @argc: argument count (not used)
 * @argv: argument values (not used)
 * @env: environ variables
 *
 * Descrpition: command interpretor
 * Return: int
 */
int main(int argc, char **argv, char **env)
{
	size_t arg_count = 0;
	int count_command = 0, status = 0, i;
	char **arguments = NULL, *command_line = NULL, *full_path;

	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);
		if (getline(&command_line, &arg_count, stdin) == -1)
			break;
		arguments = parse_command_line(command_line, arguments);
		if (arguments != NULL)
		{
			count_command++;
			i = 0;
			if (strcmp(arguments[0], "env") == 0)
				while (env[i]) /*print the environment*/
					printf("%s\n", env[i++]);
			else /*test exit*/
			{
				if (strcmp(arguments[0], "exit") == 0)
					break;
				full_path = get_path(arguments[0], env);

				status = exec_cmd(full_path, arguments, env, argv, &count_command);
			}
			free_args(arguments);
		}
		arguments = NULL;
	}
	free_args(arguments);
	free(command_line);
	return (status);
}
