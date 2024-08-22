/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:27:21 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 11:20:53 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int msh_unset(char *var_name, t_vlst **head)
{
    t_vlst		*current;
    t_vlst		*previous;
	
	current = *head;
	previous = NULL;
    while (current)
	{
        if (ft_strcmp(current->var_name, var_name) == 0)
		{
            if (previous)
                previous->next = current->next;
            else
                *head = current->next;
            free(current->var_name);
            free(current->var_value);
            free(current);
            return (0);
        }
        previous = current;
        current = current->next;
    }
    return (-1);
}
