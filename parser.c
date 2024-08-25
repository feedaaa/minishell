
// Still need to do the function that seperates the commands and the operator
#include "minishell.h"

t_statement	*parser(char *input)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
	size_t		j;

	parsed = tokenize_input(input);
	// free(input);
	temp = p_new_node(get_argc(&parsed[0]));
		// create a new node for the first argument
	head = temp;
	i = 0;
	while (parsed[i]) // goes through the first token
	{
		j = 0;
		while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		temp->argv[j] = NULL;
			// to know its the end or there is an operator after
		if (!parsed[i])
			break ;
		temp->operator= get_operator(parsed[i++]);
		temp->next = p_new_node(get_argc(&parsed[j]));
			// creatinmg a node to connect the rest of the parsed part
		temp = temp->next;
	}
	temp->next = NULL;
	// free(parsed);
	return (head);
}

t_statement	*p_new_node(int argc)
{
	t_statement	*new_node;

	new_node = malloc(sizeof(t_statement));
	new_node->argc = argc;
	new_node->argv = malloc((argc + 1) * sizeof(char *));
	new_node->operator= NONE;
	new_node->next = NULL;
	return (new_node);
}
