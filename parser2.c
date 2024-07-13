#include "minishell.h"

//Example:
//ls -l | grep something | wc -l
//parsed = ["ls", "-l", "|", "grep", "something", "|", "wc", "-l"]
//statements[0].argv= ls -l
//statements[0].operator = |
//statements[1].argv = grep something
//statements[1].operator = |
//statements[2].argv = wc -l
//statements[1].operator = NONE
// I also made it pass the size of the command line which is the number of tokens just so if you needed to use it in your part!



t_statement *parser(char *input, int *size)
{
    char **parsed;
    t_statement *statements;
    size_t i;
    size_t j;
    int k;
    size_t idx;
    size_t idx2;
    int tokens_count;

    idx = 0;
    parsed = tokenize_input(input);     //split the input string into an array of strings, stored in parsed

    tokens_count = count_tokens(*parsed);
    statements = malloc((tokens_count + 1) * sizeof(t_statement));
    if (!statements) 
    {
         while (parsed[idx2]) 
    {
        free(parsed[idx2]);
        idx2++;
    }

        free(parsed);
        return NULL;
    }

    *size = tokens_count;
    i = 0;
    j = 0;
    while (parsed[i])
    {
        statements[j].argc = get_argc(&parsed[i]);
        statements[j].argv = malloc((statements[j].argc + 1) * sizeof(char *));
        if (!statements[j].argv) 
        {
            while (idx < j) 
            {
                free(statements[idx].argv);
                idx++;
            }
            free(statements);
            while (parsed[idx2]) 
            {
                free(parsed[idx2]);
                idx2++;
            }

            free(parsed);
            return NULL;
        }

        statements[j].operator = NONE;  //initialize
        statements[j].argv[statements[j].argc] = NULL;
        k = 0;
        while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]) && (k < statements[j].argc)) 
        {
            statements[j].argv[k] = remove_quotes(parsed[i++]);  //in case there are quotes in the argument
            k++;
        }
        if (!parsed[i])
            break;
        statements[j].operator = get_operator(parsed[i++]);
        j++;
    }
    free(parsed);
    return statements;
}
