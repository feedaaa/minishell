/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/07/06 16:00:17 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//build a basic shell and then add on

void display_prompt(void)
{
	printf("minishell_test %% ");
}

void read_command(char *command, size_t size) {
    if (fgets(command, size, stdin) == NULL) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS);
        } else {
            printf("Error while reading input.\n");
            exit(EXIT_FAILURE);
        }
    }
    command[strcspn(command, "\n")] = '\0'; // Remove newline
}

void execute_command(const char *command) 
{
    pid_t child_pid = fork();

    if (child_pid == -1) 
	{
        printf("Error forking process.\n");
        exit(EXIT_FAILURE);
    } 
	else if (child_pid == 0) 
	{
        char *args[128];
        int arg_count = 0;

        char *token = strtok((char *)command, " ");
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        execvp(args[0], args);
        printf("Error executing command.\n");
        exit(EXIT_FAILURE);
    } 
	else 
        wait(NULL);
}


int main(int ac, char **av, char **env)
{
	char command[120];

	while (true)
	{
		display_prompt();
		read_command(command, sizeof(command));
		execute_command(av[1]);
	}
	return (0);
}
