/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:41:16 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 11:50:39 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void msh_echo(t_statement *command)
{
    int i = 1;
    int n = 0;

    while (command->argv[i] != NULL)
    {
        if (strcmp(command->argv[i], "-n") == 0)
            n = 1;
        else
        {
            ft_putstr_fd(command->argv[i], STDOUT_FILENO);
            if (command->argv[i + 1] != NULL)
                ft_putstr_fd(" ", STDOUT_FILENO);
        }
        i++;
    }
    if (!n)
        ft_putstr_fd("\n", STDOUT_FILENO);
}