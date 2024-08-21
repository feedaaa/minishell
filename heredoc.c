/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:15 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 18:47:02 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int here_doc(t_main *data, t_statement *node)
{
    pid_t pid;
    int fd;
    char *tmp_file;
    
    tmp_file = "/tmp/heredoc_tmp";
    fd = open(tmp_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (perror("open: "), -1);
	hd_child(data, node, fd);
    fd = open(tmp_file, O_RDONLY);
    if (fd == -1)
        return(perror("open: "), -1);
    pid = fork();
    if (pid == -1)
        return(perror("fork"), close(fd));
    if (pid == 0)
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
		execve("/bin/cat", node->argv, NULL);
    }
    else
    {
        close(fd);
        wait(NULL);
    }
	return (0);
}

int hd_child(t_statement *node, int fd)
{
    char *line;
	size_t len;
    char *delimiter;
	
	len = 0;
	line = NULL;
    delimiter = node->next->argv[1];
	while (1)
    {
        printf("> ");
        if (get_next_line(stdin) == -1)
        {
            perror("get_next_line");
            return(free(line), close(fd), -1);
        }
        if (ft_strcmp(line, delimiter) == 0)
            break;
        write(fd, line, ft_strlen(line));
        free(line);
        line = NULL;
        len = 0;
    }
    free(line);
	close(fd);
}
