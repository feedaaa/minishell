#include "minishell.h"

void	init_oldpwd(t_vlst **head)     //to create OLDPWD for 'cd -' command.. it saves the old directory on OLDPWD env variable
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
		if (streq(var_name, temp->next->var_name))  //checks if the environmnet variable and the next env variable are the same
		{
			next_node = temp->next->next;
			free(temp->next->var_name);     //delete the next var-name
			free(temp->next->var_value);   ////delete the next var-value
			free(temp->next);
			temp->next = next_node;       //connect to the one after the next node
			break ;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

//This function is responsible for printing an error message when an invalid environment variable name is encountered.
void	invalid_identifer(char *var_name)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(var_name, STDERR_FILENO);
	ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
}

int	save_user_vars(char *statement, t_vlst **head, bool to_export)
{
	char	**line;

	line = split_envp(statement);// line[0]= Variable line[1]=value
	if (get_exported_state(line[0], head) && !to_export) //this function get_exported_state checks if this Variable is exported or not
		to_export = true;
	unset_var(line[0], head);  //it needs to be deleted to be updated
	v_lstadd_back(head, v_new_node(line[0], line[1], to_export));
	free(line);
	return (EXIT_SUCCESS);
}

//checks if the env exported or not
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

t_vlst	*v_lstlast(t_vlst *node)
{
	while (node)
	{
		if (!node->next)
			break ;
		node = node->next;
	}
	return (node);
}
