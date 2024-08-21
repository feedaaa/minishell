/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:07:38 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 12:41:48 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void msh_cd(t_statement *command)
{
    char *path;
    char **env;

    if (!command->argv[1])
    {
        path = getenv("HOME");
        if (!path)
            perror("getenv");
    }
    else
        path = command->argv[1];

    if (chdir(path) == -1)
        perror("chdir");
    else
    {
        char cwd[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        setenv("PWD", cwd, 1);
        for (int i = 0; env[i]; i++)
        {
            if (strcmp(env[i], "PWD=*") == 0)
            {
                free(env[i]);
                env[i] = malloc(strlen("PWD=") + strlen(cwd) + 1);
                sprintf(env[i], "PWD=%s", cwd);
                break;
            }
        }
    }
}
