/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:52:57 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:02:25 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	msh_env(t_vlst *env)
{
	t_vlst	*envp;

	envp = env;
	while (envp)
	{
		printf("%s=%s\n", envp->var_name, envp->var_value);
		envp = envp->next;
	}
}

char	*msh_getenv(t_vlst *env, const char *var_name)
{
	t_vlst	*envp;

	envp = env;
	while (envp)
	{
		if (ft_strcmp(envp->var_name, var_name) == 0)
			return (envp->var_value);
		envp = envp->next;
	}
	return (NULL);
}

void    msh_setenv(t_vlst **env, const char *var_name, const char *var_value)
{
    t_vlst    *envp;

    envp = *env;
    while (envp)
    {
        if (ft_strcmp(envp->var_name, var_name) == 0)
        {
            free(envp->var_value);
            envp->var_value = ft_strdup(var_value);
            return ;
        }
        envp = envp->next;
    }
    envp = msh_lstnew(ft_strdup(var_name), ft_strdup(envp->var_value));
    msh_lstadd_back(env, envp);
}

void	msh_unsetenv(t_vlst **env, const char *var_name)
{
	t_vlst	*envp;
	t_vlst	*prev;

	envp = *env;
	prev = NULL;
	while (envp)
	{
		if (ft_strcmp(envp->var_name, var_name) == 0)
		{
			if (prev)
				prev->next = envp->next;
			else
				*env = envp->next;
			msh_lstdelone(envp, free);
			return ;
		}
		prev = envp;
		envp = envp->next;
	}
}
