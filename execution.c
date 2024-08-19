/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:31:23 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/19 20:48:40 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *command) {
    char *builtins[] = {"cd", "pwd", "echo", "exit", NULL};

    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(command, builtins[i]) == 0) {
            return 1; // Command is a built-in
        }
    }

    return 0; // Command is not a built-in
}

void execute_builtin(char *command, char **args)
{
    if (strcmp(command, "cd") == 0) {
        if (args[1] == NULL) {
            // Change to home directory
            chdir(getenv("HOME"));
        } else {
            // Change to specified directory
            chdir(args[1]);
        }
    } else if (strcmp(command, "pwd") == 0) {
        // Print current working directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    } else if (strcmp(command, "echo") == 0) {
        // Print arguments
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    } else if (strcmp(command, "exit") == 0) {
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
        execute_builtin(parsed_commands->argv[0], parsed_commands->argv);
        return ;
    }
    pid = fork();
    if(pid == -1)
        return (perror("fork()"));
    if (pid == 0)
        execute_command(parsed_commands->argv[0], parsed_commands->argv, env);
	// waitpid(pid, &status, 0);
	// if	(WIFEXITED(status))
	// 	g_exit_status = WEXITSTATUS(status);
}


void execute_command(char *command, char **args, char **env) 
{
    if(execve(command, args, env) == -1)
	{
		//print error message
		//exit exit failure	
	}
	
    
}
