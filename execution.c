/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:31:23 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 12:27:44 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_one_cmd(t_statement *parsed_command, char **env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        return (perror("fork()"));
    if (pid == 0)
    {
        execute_command(parsed_command->argv[0], parsed_command->argv, env);
        exit(EXIT_FAILURE);  // If exec fails
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
}

void execution(t_statement *parsed_commands, char **env)
{
    if (is_builtin(parsed_commands->argv[0]))
        execute_builtin(parsed_commands);

    if (parsed_commands->next != NULL)
    {
        execute_pipe(parsed_commands, env);
        execution(parsed_commands->next, env);
    }
    else
        exec_one_cmd(parsed_commands, env);
}

void execute_command(char *command, char **args, char **env)
{
    char *path = malloc(strlen(command) + 1);
    ft_strcpy(path, command);

    if (access(path, X_OK) == -1)
    {
        perror("access");
        free(path);
        exit(EXIT_FAILURE);
    }

    if (execve(path, args, env) == -1)
    {
        perror("execve");
        free(path);
        exit(EXIT_FAILURE);
    }
}
