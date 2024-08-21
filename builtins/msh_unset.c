/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:27:21 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 14:28:05 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	msh_unsetenv(const char *name)
{
	extern char **environ;
	char **envp;
	char *var_name;
	size_t len;

	len = ft_strlen(name);
	envp = environ;
	while (*envp)
	{
		var_name = ft_substr(*envp, 0, ft_strchr(*envp, '=') - *envp);
		if (ft_strcmp(var_name, name) == 0)
		{
			free(var_name);
			*envp = NULL;
			return ;
		}
		free(var_name);
		envp++;
	}
}
