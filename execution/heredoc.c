/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:15 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/25 21:21:24 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_heredoc(int write_fd, char *delimiter)
{
	char	*line;

	close(write_fd);
	while (1)
	{
		printf("the funtcion does come here\n");
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
			break ;
		write(write_fd, line, strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	free(line);
	close(write_fd);
	exit(EXIT_SUCCESS);
}

int	handle_heredoc(t_statement *cmd, char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;
	
	(void)cmd;
	pid = fork();
	if (pipe(pipefd) == -1 || pid < 0)
		return (perror("minishell"), -1);
	if (pid == 0)
		child_heredoc(pipefd[1], delimiter);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	return (0);
}
