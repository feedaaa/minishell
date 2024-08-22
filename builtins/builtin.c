/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:38:40 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 13:57:34 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *command)
{
	if (!ft_strncmp(command, "exit ", 5)
		|| !ft_strncmp(command, "env ", 4)
		|| !ft_strncmp(command, "export ", 7)
		|| !ft_strncmp(command, "echo ", 5)
		|| !ft_strncmp(command, "pwd ", 4)
		|| !ft_strncmp(command, "cd ", 3)
		|| !ft_strncmp(command, "unset", 6))
		return (1);
	return (0);
}

void execute_builtin(t_statement *command)
{
	t_vlst *current;
    if (strcmp(command->argv[0], "cd") == 0)
        msh_cd(command);
    else if (strcmp(command->argv[0], "pwd") == 0)
        msh_pwd(command);
    else if (strcmp(command->argv[0], "echo") == 0)
        msh_echo(command);
    else if (strcmp(command->argv[0], "env") == 0)
        msh_env(current->env);
    else if (strcmp(command->argv[0], "export") == 0)
        msh_export(current->var_name, current->var_value, current->env);
    else if (strcmp(command->argv[0], "unset") == 0)
        msh_unset(current->var_name, current->env);
//     else if(strcmp(command->argv[0], "exit") == 0)
//         msh_exit(command);
}

void execute_command(t_statement *command)
{
	if (is_builtin(command->argv[0]))
		execute_builtin(command);
}