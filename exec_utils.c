/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:51:11 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:15:00 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strcpy(char *dst, const char *src)
{
    int i;
    
    i = 0;
    while(dst[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

t_vlst	*msh_lstnew(char *var_name, char *var_value)
{
	t_vlst	*new;

	new = malloc(sizeof(t_vlst));
	if (!new)
		return (NULL);
	new->var_name = ft_strdup(var_name);
	new->var_value = ft_strdup(var_value);
	new->next = NULL;
	return (new);
}

void	msh_lstadd_back(t_vlst **lst, t_vlst *new)
{
	t_vlst	*temp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = new;
}

void	msh_lstdelone(t_vlst *lst, void (*del)(void*))
{
	if (lst == NULL || del == NULL)
		return ;
	del(lst->var_name);
	del(lst->var_value);
	free(lst);
}