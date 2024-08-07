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

long long	g_exit_status = 0;

// funtion to display prompt
// void display_prompt(void)
// {
// 	printf("minishell_test %% ");
// }

char	*get_command(void)
{
	char	*raw_input;
	char	*input;

	raw_input = readline("minishell$ ");
	input = trim_free(raw_input, " \t");
	return (input);
}

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i += 1;
	}
	if (src[i] && i < n)
	{
		while (i < n)
			dest[i++] = '\0';
	}
	dest[i] = '\0';
	return (dest);
}

//trims all the spaces and taps that are not needed at the beg and the end of a command line
char	*trim_free(char *s1, char const *set)
{
	size_t	beg;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	beg = 0;
	while (s1[beg] != '\0' && ft_strchr(set, s1[beg]) != NULL)
		beg += 1;
	end = ft_strlen(s1 + beg); //starting from s1[beg] to the end of s1.
	while (end > beg && ft_strchr(set, s1[(beg + end) - 1]) != NULL) //s1[(beg + end) - 1 to take in consider the begining char without spaces
		end -= 1;
	trimmed_str = malloc((end + 1) * sizeof(char));
	if (!trimmed_str)
		return (NULL);
	ft_strncpy(trimmed_str, (s1 + beg), end);  //it copies the updated s1 with size of the updated end(no spaces) 
	free(s1);
	return (trimmed_str);
}

int main(int ac, char **av, char **env)
{
	char *command;
	t_data		data;
	t_statement  *parsed_commands;
	int size;
	size = 0;
	setup_shell(env, &data, &parsed_commands);
	while (true)
	{
		command = get_command();
		add_history(command);
		// display_prompt();
		// read_command(command, sizeof(command));
		command = expander(command, &data);
		parsed_commands = parser(command, &size);
		// execute_command(parsed_commands, env);
		// execute_command(av[1], env);
		clean_parsed(parsed_commands, env); //to clear everything and handle the next command 
	}
	return (0);
}

// int main(int ac, char **av, char **env)
// {
// 	t_info		*data = NULL;

// 	(void)av;
// 	if(ac != 1)
// 		//return error;
// 	// finding the env
// 	data->env = array_duplicate(env, array_len(env) + 1);
// 	// if(!env)
// 	// 	return ;
// 	//find pwd
// 	// get_pwd(&data);
// 	// init_data(&data);
// 	//minishell loop
// }
