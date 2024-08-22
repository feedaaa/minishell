/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:05 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 13:21:03 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// Function to handle redirections before executing a command
int handle_redirections(t_statement *cmd)
{
    int fd;

    // Output redirection (overwrite, e.g., `>`)
    if (cmd->operator == RDR_OUT_REPLACE) {
        fd = open(cmd->argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("minishell");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    // Output redirection (append, e.g., `>>`)
    else if (cmd->operator == RDR_OUT_APPEND)
	{
        fd = open(cmd->argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
		{
            perror("minishell");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    // Input redirection (e.g., `<`)
    else if (cmd->operator == RDR_INPUT) {
        fd = open(cmd->argv[1], O_RDONLY);
        if (fd < 0)
		{
            perror("minishell");
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return (0);
}

