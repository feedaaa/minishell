/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:52:40 by ffidha            #+#    #+#             */
/*   Updated: 2024/07/15 11:38:38 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

//function for getting the array
size_t array_len(char **array)
{
	size_t		len;
	
	len = 0;
	while(array[len])
		len++;
	return (len);
}

//duplicate the array
char **array_duplicate(char **array, size_t len)
{
	char		**new;
	size_t		i;

	new = malloc(sizeof(char *) * len);
	if (!new)
		return(NULL);
	i = 0;
	while(array[i] && i < len)
	{
		new[i] = ft_strdup(array[i]);
		if(!new[i])
		{
			while(--i < len)
				free(new[i]);
			free(new);
			return (NULL);
		}
		i++;
	}
	while(i < len)
		new[i++] = NULL;
	return (new);
}