/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:38:01 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/19 13:59:00 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int msh_pwd(t_statement *command)
{
	char cwd[1024];
	
	if(getcwd(cwd, 1024))
	{
		ft_putendl_fd(cwd, 0);
		return (0);
	}
	else
	{
		ft_putstr_fd("msh: pwd: ", 2);
		ft_putstr_fd("minishell: pwd error", 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
}