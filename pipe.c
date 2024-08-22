/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:33:25 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 13:23:18 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// void execute_pipe(t_statement *parsed_commands, t_main *data)
// {
//     if (pipe(data->pipe_fds) == -1)
//         return (perror("pipe()"));
//     pid_t pid = fork();
//     if (pid == -1)
//         return (perror("fork()"));
//     if (pid == 0)
//     {
//         close(data->pipe_fds[0]);
//         dup2(data->pipe_fds[1], STDOUT_FILENO);
//         close(data->pipe_fds[1]);
//         execute_command(parsed_commands->argv[0], parsed_commands->argv, data->env);
//         exit(EXIT_FAILURE);  // If exec fails
//     }
//     close(data->pipe_fds[1]);
//     dup2(data->pipe_fds[0], STDIN_FILENO);
//     close(data->pipe_fds[0]);
// }
