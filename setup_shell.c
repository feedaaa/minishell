#include "minishell.h"

// function to caonnect as a linked list
t_vlst	*v_new_node(char *var_name, char *var_value, bool is_exported)
{
	t_vlst	*new_node;

	new_node = malloc(sizeof(t_vlst));
	new_node->var_name = var_name;
	new_node->var_value = var_value;
	new_node->is_exported = is_exported;
	new_node->next = NULL;
	return (new_node);
}

// This function splits an environment variable string into its name and value components,
// storing them in a dynamically allocated array of strings.

char	**split_envp(char *env)
{
	char	**splitted;
	size_t	indexof_equal;

	splitted = malloc(3 * sizeof(char *));
	// 3 because we have the 1-name variable and 2-the expanded value and 3-the null
	if (!splitted)
		return (NULL);
	indexof_equal = ft_strchr(env, '=') - env;
	splitted[0] = ft_substr(env, 0,
			indexof_equal);                                 
		//"NAME"starting from 0 to where there was equal sign
	splitted[1] = ft_substr(env, indexof_equal + 1,
			ft_strlen(&env[indexof_equal]));
		//"VALUE" starting from the equal sign and afterwards
	splitted[2] = NULL;                                                             
		// To indicate the end of the first environment variable
	return (splitted);
}

// initializes a linked list of environment variables from an array of environment strings
t_vlst	*init_envp_lst(char **envp)
{
	t_vlst	*head;
	t_vlst	*temp;
	char	**line;
	size_t	i;

	line = split_envp(envp[0]);
	// splits the first environment variable into array of name ane the other array is the value if we assume NAME=VALUE
	head = v_new_node(line[0], line[1], true);
	free(line);
	i = 1;
	temp = head;
	while (envp[i])
	{
		line = split_envp(envp[i]);
		temp->next = v_new_node(line[0], line[1], true);
		free(line);
		temp = temp->next;
		i += 1;
	}
	init_oldpwd(&head);
	return (head);
}

// initialising the shell based on the environment
void	setup_shell(char **envp, t_data *data, t_statement **statement_list)
{
	data->envp = envp;
	data->envp_lst = init_envp_lst(envp);
	data->head = NULL;
	*statement_list = NULL;
	config_signals();
		// this function to update the shell depending on some signals such as ctrl+c ...
}
