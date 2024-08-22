/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:15:10 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 11:17:02 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	msh_export(char *var_name, char *var_value, t_vlst **head)
{
	t_vlst *current;
	t_vlst *new_var;

	current = *head;
	while (current)
	{
		if (ft_strcmp(current->var_name, var_name) == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(var_value);
			current->is_exported = true;
			return (0);
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_vlst));
	new_var->var_name = ft_strdup(var_name);
	new_var->var_value = ft_strdup(var_value);
	new_var->is_exported = true;
	new_var->next = *head;
	*head = new_var;
	return (0);
}