/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/07/06 20:50:57 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//build a basic shell and then add on

// funtion to display prompt
void display_prompt(void)
{
	printf("minishell_test %% ");
}

// function to read command from command line
void read_command(char *command, size_t size) 
{
	// readline function is bascially getnextline on crack
  char *line = readline("$> ");
  if (line == NULL) {
    if (feof(stdin)) {
      printf("\n");
      exit(EXIT_SUCCESS);
    } else {
      printf("Error reading input.\n");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(line, "") == 0) {
    free(line);
    return;
  }

  if (strlen(line) >= size) {
    printf("Error: Command too long.\n");
    free(line);
    return;
  }
  strcpy(command, line);
  free(line);
}

// function to fork the process and execute the process
void execute_command(const char *command, char **env) 
{
    pid_t child_pid = fork(); // forks a child process
    if (child_pid == -1) 
	{
        printf("Error forking process.\n");
        exit(EXIT_FAILURE);
    } 
	else if (child_pid == 0) 
	{
        char *args[128];
        int arg_count = 0;

		//strtok tokenizes(splits the command into smaller pieces) the command.
		// we are not supposed to use this function but for understanding purposes huhu.
        char *token = strtok((char *)command, " ");
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " "); 
        }
        args[arg_count] = NULL;

        execvp(args[0], args); // have to use execve instead
        printf("Error executing command.\n");
        exit(EXIT_FAILURE);
    } 
	else 
        wait(NULL);
}


int main(int ac, char **av, char **env)
{
	char command[120];
	t_commandinput  parsed_commands;

	while (true)
	{
		display_prompt();
		read_command(command, sizeof(command));
		 parsed_commands = parser(command);
		// execute_command(parsed_commands, env);
		execute_command(av[1], env);
		clean_parsed(parsed_commands, env); //to clear everything and handle the next command 
	}
	return (0);
}
