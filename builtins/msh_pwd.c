/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:38:01 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/12 14:22:50 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int msh_pwd(t_shell *shell)
{
	char str[1024];
	
	if(getcwd(str, 1024))
	{
		ft_putendl_fd(str, 0);
		return (0);
	}
	else
		return (1);
}