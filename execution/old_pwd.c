#include "../minishell.h"

void	init_oldpwd(t_vlst **head)
{
	char	*temp;

	unset_var("OLDPWD", head);
	temp = ft_strjoin("OLDPWD=", getenv("HOME"));
	save_user_vars(temp, head, true);
	free(temp);
}

int	unset_var(char *var_name, t_vlst **head)
{
	t_vlst	*temp;
	t_vlst	*next_node;

	temp = *head;
	if (ft_strchr(var_name, '='))
	{
		invalid_identifer(var_name);
		return (EXIT_FAILURE);
	}
	while (temp && temp->next != NULL)
	{
		if (streq(var_name, temp->next->var_name))
		{
			next_node = temp->next->next;
			free(temp->next->var_name);
			free(temp->next->var_value);
			free(temp->next);
			temp->next = next_node;
			break ;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

int	save_user_vars(char *statement, t_vlst **head, bool to_export)
{
	char	**line;

	line = split_envp(statement);
	if (get_exported_state(line[0], head) && !to_export)
		to_export = true;
	unset_var(line[0], head);
	v_lstadd_back(head, v_new_node(line[0], line[1], to_export));
	return (EXIT_SUCCESS);
}

bool	get_exported_state(char *var_name, t_vlst **head)
{
	t_vlst	*temp;

	temp = *head;
	while (temp != NULL)
	{
		if (streq(var_name, temp->var_name))
			return (temp->is_exported);
		temp = temp->next;
	}
	return (false);
}

void	v_lstadd_back(t_vlst **head, t_vlst *new)
{
	t_vlst	*temp;

	if (head)
	{
		if (!*head)
			*head = new;
		else
		{
			temp = v_lstlast(*(head));
			temp->next = new;
		}
	}
}
