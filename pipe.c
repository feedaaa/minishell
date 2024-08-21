/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:33:25 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 18:41:00 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipe(t_statement *parsed_commands, char **env)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return (perror("pipe()"));
    pid_t pid = fork();
    if (pid == -1)
        return (perror("fork()"));
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_command(parsed_commands->argv[0], parsed_commands->argv, env);
        exit(EXIT_FAILURE);  // If exec fails
    }
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}


