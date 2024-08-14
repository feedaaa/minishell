
/*aysha header*/

#include "minishell.h"


//Example:
//ls -l | grep something | wc -l
//parsed = ["ls", "-l", "|", "grep", "something", "|", "wc", "-l"]
//statements[0].argv= ls -l
//statements[0].operator = |
//statements[1].argv = grep something
//statements[1].operator = |
//statements[2].argv = wc -l
//statements[2].operator = NONE
// I also made it pass the size of the command line which is the number of tokens just so if you needed to use it in your part!



// t_statement *parser(char *input, int *size)
// {
//     char **parsed;
//     t_statement *statements;
//     size_t i;
//     size_t j;
//     int k;
//     size_t idx;
//     size_t idx2 = 0;
//     int tokens_count;

//     idx = 0;
//     parsed = tokenize_input(input);     //split the input string into an array of strings, stored in parsed

//     tokens_count = count_tokens(*parsed);
//     statements = malloc((tokens_count + 1) * sizeof(t_statement));
//     if (!statements) 
//     {
//          while (parsed[idx2]) 
//     {
//         free(parsed[idx2]);
//         idx2++;
//     }

//         free(parsed);
//         return NULL;
//     }

//     *size = tokens_count;
//     i = 0;
//     j = 0;
//     while (parsed[i])
//     {
//         statements[j].argc = get_argc(&parsed[i]);
//         statements[j].argv = malloc((statements[j].argc + 1) * sizeof(char *));
//         if (!statements[j].argv) 
//         {
//             while (idx < j) 
//             {
//                 free(statements[idx].argv);
//                 idx++;
//             }
//             free(statements);
//             while (parsed[idx2]) 
//             {
//                 free(parsed[idx2]);
//                 idx2++;
//             }

//             free(parsed);
//             return NULL;
//         }

//         statements[j].operator = NONE;  //initialize
//         statements[j].argv[statements[j].argc] = NULL;
//         k = 0;
//         while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]) && (k < statements[j].argc)) 
//         {
//             statements[j].argv[k] = remove_quotes(parsed[i++]);  //in case there are quotes in the argument
//             k++;
//         }
//         if (!parsed[i])
//             break;
//         statements[j].operator = get_operator(parsed[i++]);
//         j++;
//     }
//     free(parsed);
//     return statements;
// }
//Example: "echo hello | grep world"
// temp->argv[0] = "echo";
// temp->argv[1] = "hello";
// temp->argv[2] = NULL;
// temp->operator = PIPE;
// temp->argv[0] = "grep";
// temp->argv[1] = "world";
// temp->argv[2] = NULL;

t_statement			*parser(char *input)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
    size_t      j;

	parsed = tokenize_input(input);
	free(input);
	temp = p_new_node(get_argc(&parsed[0]));//create a new node for the first argument
	head = temp;
	i = 0;
	while (parsed[i])//goes through the first token
	{
		j = 0;
		while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		temp->argv[j] = NULL;   //to know its the end or there is an operator after
		if (!parsed[i])
			break ;
		temp->operator = get_operator(parsed[i++]);
		temp->next = p_new_node(get_argc(&parsed[j]));// creatinmg a node to connect the rest of the parsed part
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
