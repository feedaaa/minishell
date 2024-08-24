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
#include "minishell.h"

//reads whats inside fd[0] and write to fd[1]
static void	redirect_input_until(t_statement *node)
{
	char	*buff;
	int		fd[2];

	pipe(fd);  //for one file to use the others file input
	while (1)
	{
		buff = readline("> ");  //store each line of input
		if (streq(buff, node->next->argv[0])) //any of the buff is similar to the next node it breaks
			break ;
		ft_putendl_fd(buff, fd[1]); //writes the input stored in buff inside fd1
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); //what is store in fd[0] is the input
	close(fd[0]);
	free(buff);
}

//read from file insead from the standard input "<"
static void	redirect_input(t_statement *node)
{
	int		in_file;
	char	*error_msg_prefix;

	if (node->next->argv[0])  //the first argument of the next node
	{
		while (node->next->operator == RDR_INPUT)  //in case there is more than one redirector 
			node = node->next;                  //it reaches to the last redirector command
		if (access(node->next->argv[0], F_OK) == 0) //checks if the file exists
		{
			in_file = open(node->next->argv[0], O_RDONLY, 0666);
			dup2(in_file, STDIN_FILENO); //makes the file descriptor the standard input
		}
		else   //if the file does not exist
		{
			error_msg_prefix = ft_strjoin("minishell: ", node->next->argv[0]);
			perror(error_msg_prefix);
			free(error_msg_prefix);
			g_exit_status = 2;
			exit(EXIT_FAILURE);
		}
	}
}

// O_WRONLY: Open the file for writing only.
// O_TRUNC: remove any content if exists.
// O_CREAT: Create the file if it doesn’t exist.
// 0666: Set the file’s permissions (readable and writable by everyone).
// O_APPEND: keeps any content that exist.
static void	redirect_output(t_statement *node)
{
	close(STDOUT_FILENO); //close the standard output file so than the next opened file will take its job
	while (node->next->operator == RDR_OUT_REPLACE
		|| node->next->operator == RDR_OUT_APPEND)
	{
		if (node->operator == RDR_OUT_REPLACE) //">"
			open(node->next->argv[0], O_WRONLY | O_TRUNC | O_CREAT, 0666); //so the standart output will appear in this file replacing its content
		else if (node->operator == RDR_OUT_APPEND) //">>"
			open(node->next->argv[0], O_WRONLY | O_APPEND | O_CREAT, 0666);
		node = node->next;
		close(1);
	}
	if (node->operator == RDR_OUT_REPLACE)
		open(node->next->argv[0], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (node->operator == RDR_OUT_APPEND)
		open(node->next->argv[0], O_WRONLY | O_APPEND | O_CREAT, 0666);
}

void	exec_redirects(t_statement *node, t_data *data)
{
	t_statement	*temp;

	temp = node;
	if (node->operator == RDR_INPUT)
		redirect_input(node);
	else if (node->operator == RDR_INPUT_UNTIL)
		redirect_input_until(node);
	else
		redirect_output(node);
	temp->operator = NONE; //to not mistakenly reprocess the redirection
	while (node->operator != NONE && node->operator != PIPE)
		node = node->next;
	if (node->operator == NONE)
		exec_cmd(temp, data);
	else
		exec_pipe(node, data);
}



// // Function to handle redirections before executing a command
// int handle_redirections(t_statement *cmd)
// {
//     int fd;

//     // Output redirection (overwrite, e.g., `>`)
//     if (cmd->operator == RDR_OUT_REPLACE) {
//         fd = open(cmd->argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd < 0) {
//             perror("minishell");
//             return -1;
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
//     // Output redirection (append, e.g., `>>`)
//     else if (cmd->operator == RDR_OUT_APPEND)
// 	{
//         fd = open(cmd->argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (fd < 0)
// 		{
//             perror("minishell");
//             return -1;
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
//     // Input redirection (e.g., `<`)
//     else if (cmd->operator == RDR_INPUT) {
//         fd = open(cmd->argv[1], O_RDONLY);
//         if (fd < 0)
// 		{
//             perror("minishell");
//             return -1;
//         }
//         dup2(fd, STDIN_FILENO);
//         close(fd);
//     }
//     return (0);
// }

