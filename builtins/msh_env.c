/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:52:57 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 11:19:12 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	msh_env(t_vlst *head)
{
    t_vlst *current;

	current = head;
    while (current)
	{
        if (current->is_exported) {
            printf("%s=%s\n", current->var_name, current->var_value);
        }
        current = current->next;
    }
    return (0);
}
