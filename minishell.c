/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/14 12:10:22 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_statement			*parser(char *input, int *size)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
    size_t      j;

	parsed = tokenize_input(input);
	free(input);
	temp = p_new_node(get_argc(&parsed[0]));//create a new node for the first argument
	head = temp;
	i = 0;
	while (parsed[i])//goes through the first token
	{
		j = 0;
		while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		temp->argv[j] = NULL;   //to know its the end or there is an operator after
		if (!parsed[i])
			break ;
		temp->operator = get_operator(parsed[i++]);
		temp->next = p_new_node(get_argc(&parsed[j]));// creatinmg a node to connect the rest of the parsed part
		temp = temp->next;
	}
	temp->next = NULL;
	free(parsed);
	return (head);
}

void	setup_shell(char **envp, t_data *data, t_statement **statement_list)
{
	data->envp = envp;
	data->envp_lst = init_envp_lst(envp);
	data->head = NULL;
	*statement_list = NULL;
	// config_signals(); //this function to update the shell depending on some signals such as ctrl+c ...
}

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
