/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:07:38 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/19 14:03:36 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int msh_cd(t_shell *shell, int i, int args)
{
    char *path;

    if (args == 1)
    {
        // Change to home directory
        path = getenv("HOME");
        if (path == NULL)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            return (1);
        }
    }
    else
    {
        // Change to specified directory
        path = shell->cmd[i].args[1];
    }

    if (chdir(path) == -1)
    {
        ft_putstr_fd("msh: cd: ", 2);
        return (1);
    }
    update_pwd(shell);
    return (0);
}

void update_pwd(t_shell *shell)
{
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);
    setenv("OLDPWD", shell->pwd, 1);
    shell->pwd = ft_strdup(cwd);
}