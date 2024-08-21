/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 18:41:46 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
long long	g_exit_status = 0;

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

static char	*trim_free(char *s1, char const *set)
{
	size_t	beg;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	beg = 0;
	while (s1[beg] != '\0' && ft_strchr(set, s1[beg]) != NULL)
		beg += 1;
	end = ft_strlen(s1 + beg);
	while (end > beg && ft_strchr(set, s1[(beg + end) - 1]) != NULL)
		end -= 1;
	trimmed_str = malloc((end + 1) * sizeof(char));
	if (!trimmed_str)
		return (NULL);
	ft_strncpy(trimmed_str, (s1 + beg), end);
	free(s1);
	return (trimmed_str);
}

static char	*get_input(void)
{
	char	*raw_input;
	char	*input;

	raw_input = readline("minishell$ ");
	input = trim_free(raw_input, " \t");
	return (input);
}

void	print_tokens(t_statement *list)
{
	t_statement	*temp = list;

	while (temp)
	{
		int i = -1;
		if (temp->argv)
		{
			while (temp->argv[++i])
				printf("%s ", temp->argv[i]);
			printf("\n");
		}
		temp = temp->next;
	}
}

int main(int ac, char **av, char **env)
{
	char *command;
	(void)av;
	(void)ac;
	t_data		data;
	t_statement  *parsed_commands;
	int size;
	size = 0;
	setup_shell(env, &data, &parsed_commands);
	while (true)
	{
		command = get_input();
		if (!valid_input(command, &data))    //checks if there are any syntax errors
			continue ;
		add_history(command);
		command = expander(command, &data);
    if (!command[0])
		{
			free(command);
			continue ;
		}
    parsed_commands = parser(command);
	print_tokens(parsed_commands);
		execution(parsed_commands, env);
		// execute_command(av[1], env); //to do
		clean_parsed(&parsed_commands, &data); //to clear/free everything and handle the next command 
	}
	return (0);
}

