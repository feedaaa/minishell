/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:15 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 13:22:42 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_heredoc(t_statement *cmd, char *delimiter)
{
    int pipefd[2];
    pid_t pid;
    char *line;

    if (pipe(pipefd) == -1)
	{
        perror("minishell");
        return -1;
    }
    pid = fork();
    if (pid == 0)
	{
        close(pipefd[0]);
        while (1)
		{
            line = readline("> ");
            if (strcmp(line, delimiter) == 0) {
                free(line);
                break;
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
        return -1;
    }
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    waitpid(pid, NULL, 0);
    return (0);
}

// int here_doc(t_statement *node)
// {
//     pid_t pid;
//     int fd;
//     char *tmp_file;
    
//     tmp_file = "/tmp/heredoc_tmp";
//     fd = open(tmp_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return (perror("open: "), -1);
// 	hd_child(node, fd);
//     fd = open(tmp_file, O_RDONLY);
//     if (fd == -1)
//         return(perror("open: "), -1);
//     pid = fork();
//     if (pid == -1)
//         return(perror("fork"), close(fd));
//     if (pid == 0)
//     {
//         dup2(fd, STDIN_FILENO);
//         close(fd);
// 		execve("/bin/cat", node->argv, NULL);
//     }
//     close(fd);
// 	return (0);
// }

// int hd_child(t_statement *node, int fd)
// {
//     char *line;
// 	size_t len;
//     char *delimiter;
	
// 	len = 0;
// 	line = NULL;
//     delimiter = node->next->argv[1];
// 	while (1)
//     {
//         printf("> ");
//         get_next_line(fd);
//         if(!&get_next_line)
//         {
//             perror("get_next_line");
//             return(free(line), close(fd), -1);
//         }
//         if (ft_strcmp(line, delimiter) == 0)
//             break;
//         write(fd, line, ft_strlen(line));
//         free(line);
//         line = NULL;
//         len = 0;
//     }
//     free(line);
// 	return (close(fd), 0);
// }
