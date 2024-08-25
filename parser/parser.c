#include "../minishell.h"

t_statement	*parser(char *input)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
	size_t		j;

	parsed = tokenize_input(input);
	temp = p_new_node(get_argc(&parsed[0]));
	head = temp;
	i = 0;
	while (parsed[i])
	{
		j = 0;
		while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		temp->argv[j] = NULL;
		if (!parsed[i])
			break ;
		temp->operator = get_operator(parsed[i++]);
		temp->next = p_new_node(get_argc(&parsed[j]));
		temp = temp->next;
	}
	temp->next = NULL;
	free(parsed);
	return (head);
}

t_statement	*p_new_node(int argc)
{
	t_statement	*new_node;

	new_node = malloc(sizeof(t_statement));
	new_node->argc = argc;
	new_node->argv = malloc((argc + 1) * sizeof(char *));
	new_node->operator = NONE;
	new_node->next = NULL;
	return (new_node);
}
