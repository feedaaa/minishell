#include "../minishell.h"

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

char	**split_envp(char *env)
{
	char	**splitted;
	size_t	indexof_equal;

	splitted = malloc(3 * sizeof(char *));
	if (!splitted)
		return (NULL);
	indexof_equal = ft_strchr(env, '=') - env;
	splitted[0] = ft_substr(env, 0, indexof_equal);
	splitted[1] = ft_substr(env, indexof_equal + 1,
			ft_strlen(&env[indexof_equal]));
	splitted[2] = NULL;
	return (splitted);
}

t_vlst	*init_envp_lst(char **envp)
{
	t_vlst	*head;
	t_vlst	*temp;
	char	**line;
	size_t	i;

	line = split_envp(envp[0]);
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
	// init_oldpwd(&head);
	return (head);
}

void	setup_shell(char **envp, t_data *data, t_statement **statement_list)
{
	data->envp = envp;
	data->envp_lst = init_envp_lst(envp);
	data->head = NULL;
	*statement_list = NULL;
	config_signals();
}
