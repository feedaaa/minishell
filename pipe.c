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


//executing the command on the left side 
static void	left_side(t_statement *nd, t_data *data, int pdes[2])
{
	close(STDOUT_FILENO);
	dup(pdes[1]);  //duplicating pdes[1] to STDOUT_FILENO( command produces will go into the pipe instead of being printed to the terminal.)
	close(pdes[0]);
	close(pdes[1]);
	exec_cmd(nd, data);
}

//executing the command on the right side 
static void	right_side(t_statement *nd, t_data *data, int pdes[2])
{
	close(STDIN_FILENO);
	dup(pdes[0]);  //will read from pdes[0]
	close(pdes[0]);
	close(pdes[1]);
	exec_cmd(nd, data);
}

void	panic(t_data *data, char *error_msg, int exit_status)
{
	if (data)
		destroy(data);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	exit(exit_status);
}

//	bytes written on pipedes[1] can be read on pipedes[0]
void	exec_pipe(t_statement *node, t_data *data)
{
	pid_t	child_pid;
	int		pipedes[2];
	int		temp_status;

	node->operator = NONE;  // avoids reprocessing 
	if (pipe(pipedes) == -1) // sets up pipedes[0] for reading and pipedes[1] for writing
		panic(data, PIPE_ERR, EXIT_FAILURE);
	child_pid = fork();
	if (child_pid == -1)
		panic(data, FORK_ERR, EXIT_FAILURE);
	if (child_pid == 0)
		left_side(node, data, pipedes);
	right_side(node->next, data, pipedes); //the parent process go through this function
	close(pipedes[0]);
	close(pipedes[1]);
	waitpid(child_pid, &temp_status, 0);
	g_exit_status = temp_status >> 8;
}

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
