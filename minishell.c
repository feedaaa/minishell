/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/12 16:53:09 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void read_command(char *command, size_t size) 
{
  char *line = readline("$> ");
  if (line == NULL) {
    if (feof(stdin)) {
      printf("\n");
      exit(EXIT_SUCCESS);
    } else {
      printf("Error reading input.\n");
      exit(EXIT_FAILURE);
    }
    command[strcspn(command, "\n")] = '\0'; // Remove newline
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

int main(int ac, char **av, char **env)
{
	char command[120];
	t_data		data;
	t_statement  *parsed_commands;
	int size;
	size = 0;
	setup_shell(env, &data, &parsed_commands);
	while (true)
	{
		read_command(command, sizeof(command));
		parsed_commands = parser(command, &size);
		// execute_command(parsed_commands, env); //to do
		// execute_command(av[1], env); //to do
		// clean_parsed(parsed_commands, env); //to clear everything and handle the next command 
	}
	return (0);
}
