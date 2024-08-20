/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:31:23 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/20 12:28:56 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *command) {
    char *builtins[] = {"cd", "pwd", "echo", "exit", NULL};

    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(command, builtins[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

void execute_builtin(char *command)
{
    if (strcmp(command, "cd") == 0)
	{
		// msh_cd()
    }
	else if (strcmp(command, "pwd") == 0)
	{
		msh_pwd(command);
    }
	else if (strcmp(command, "echo") == 0)
	{
		// msh_echo()
    }
	else if (strcmp(command, "exit") == 0)
	{
        // Exit the shell
        exit(0);
    }
}
void    ft_execute(t_statement *parsed_commands, char **env)
{
    pid_t   pid;
	int		status;
	
    // to do: is_builtin and execute_builtin
    if(is_builtin(parsed_commands->argv[0]))
    {
        execute_builtin(parsed_commands->argv[0]);
        return ;
    }
    pid = fork();
    if(pid == -1)
        return (perror("fork()"));
    if (pid == 0)
        execute_command(parsed_commands->argv[0], parsed_commands->argv, env);
	waitpid(pid, &status, 0);
	if	(WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}


void execute_command(char *command, char **args, char **env) 
{
    if(execve(command, args, env) == -1)
	{
		//print error message
		//exit exit failure	
	}
	
    
}
