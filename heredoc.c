/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:15 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/25 12:12:05 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_statement *cmd, char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
	{
		perror("minishell");
		return (-1);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	return (0);
}