#include "../minishell.h"

static bool	handle_pipe_and_spaces(char *input, size_t *i)
{
	(*i)++;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	if (is_therechar(REDIRECTS, input[*i]))
		return (false);
	return (true);
}

static bool	check_for_operator_syntax(char *input, size_t *i)
{
	if (input[*i] == ' ')
	{
		while (input[*i] && input[*i] == ' ')
			(*i)++;
	}
	if (is_therechar(OPERATORS, input[*i]))
		return (unexpected_token(input[*i]));
	return (false);
}

static bool handle_operator(char *input, size_t *i, bool *in_quotes)
{
    if(input[*i] == input[*i + 1]) 
    {
        *i += 2;
    } 
    else if (is_therechar(OPERATORS, input[*i]) && !*in_quotes) 
    {
        if (input[*i] == '|')
        {
            if (!handle_pipe_and_spaces(input, i))
                return false;
        }
        else
        {
            (*i)++;
        }
        if (check_for_operator_syntax(input, i))
        return true;
    }
    return false;
}

bool invalid_syntax_on_operator(char *input) 
{
    size_t i = 0;
    bool in_quotes = false;

    while (has_operator(&input[i])) {
        if (is_therechar(QUOTES, input[i])) {
            in_quotes = !in_quotes;
        }
        if (handle_operator(input, &i, &in_quotes)) {
            return true;
        }
        i++;
    }
    return false;
}
