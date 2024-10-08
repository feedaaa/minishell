/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/30 21:59:40 by ffidha           ###   ########.fr       */
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

	raw_input = readline("OURSHELL -> ");
	input = trim_free(raw_input, " \t");
	return (input);
}

int	main(int ac, char **av, char **env)
{
	char		*command;
	t_data		data;
	t_statement	*parsed_commands;

	(void)av;
	(void)ac;
	if (av && ac > 1)
		panic(NULL, CL_ARGUMENTS_ERR, EXIT_FAILURE);
	setup_shell(env, &data, &parsed_commands);
	while (true)
	{
		command = get_input();
		if (!valid_input(command, &data))
			continue ;
		add_history(command);
		command = expander(command, &data);
		if (!command)
			free(command);
		parsed_commands = parser(command);
		free(command);
		data.head = parsed_commands;
		exec_type(parsed_commands, &data);
		clean_parsed(&parsed_commands, &data);
	}
}
