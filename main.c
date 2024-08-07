/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/07/15 11:35:37 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// funtion to display prompt
void display_prompt(void)
{
	printf("minishell_test %% ");
}

// int main(int ac, char **av, char *const *env)
// {
// 	char *command;
	// t_data		data;
// 	t_statement  *parsed_commands;
// 	int size;
// 	size = 0;
	// setup_shell(envp, &data, &parsed_commands);
// 	while (true)
// 	{
// 		display_prompt();
// 		read_command(command, sizeof(command));
		// command = expander(command, &data);
// 		parsed_commands = parser(command, &size);
// 		execute_command(parsed_commands, env);
// 		// execute_command(av[1], env);
// 		clean_parsed(parsed_commands, env); //to clear everything and handle the next command 
// 	}
// 	return (0);
// }

int main(int ac, char **av, char **env)
{
	t_info		*data = NULL;

	(void)av;
	if(ac != 1)
		//return error;
	// finding the env
	data->env = array_duplicate(env, array_len(env) + 1);
	// if(!env)
	// 	return ;
	//find pwd
	// get_pwd(&data);
	// init_data(&data);
	//minishell loop
}
