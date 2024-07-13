
//Still need to do the function that seperates the commands and the operator


char **tokenize_input(char *input)     // Bsaically it tokenizes the input
{

    size_t i;
    size_t token_index; 
    size_t token_count;
    size_t token_length;
    size_t j ;
    size_t k;	
    char **tokens;
	token_index = 0;
	k = 0;
	i = 0;
    token_count = count_tokens(input);
    tokens = malloc((token_count + 1) * sizeof(char *));
	if(!tokens)
		return (NULL);
    while (input[i]) 
    {
        token_length = get_token_length(&input[i]);
        if (!token_length)      //In case there is no input or total length =0 it moves to the next token by increasing i
        {
            i++;
            continue;
        }

        tokens[token_index] = malloc((token_length + 1) * sizeof(char));
	    if(!tokens[token_index])
	    {
		while(k <token_index)
		{
			free(tokens[k]);
            k++;
		}
		    free(tokens);
		    return NULL;  
	    }
        j = 0;
        while (input[i] && j < token_length)
        {
            tokens[token_index][j++] = input[i++];
        }
        tokens[token_index++][j] = '\0';
    }

    tokens[token_index] = NULL;
    return tokens;
}

//Counts how many tokens do we have
size_t	count_tokens(char *input)
{
	size_t	count;
	bool	flag;
	bool	quotes;

	count = 0;
	flag = false;
	quotes = false;
	while (*input)
	{
		if (is_therechar(OPERATORS, *input)) //Counts the operators
			count += 1;
		if (is_therechar(QUOTES, *input) && *input == *(input + 1))       //In case there is a double quotation just move 2 steps to skip them
			input += 2;
		else if (is_therechar(QUOTES, *input))
			quotes = !quotes;
		if (*input != ' ' && !is_therestr(OPERATORS, *input) && !flag && !quotes)
		{
			flag = true;
			count += 1;
		}
		else if (*input == ' ' || is_therechar(OPERATORS, *input)) //Counts nthe tokens that are seperated by spaces and operators
			flag = false;
		input += 1;
	}
	return (count);
}

//check if there str == c return true if not return false
bool	is_therechar(const char *str, int ch)
{
	size_t	i;
    i = 0;

	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ch)
			return (true);
		i++;
	}
	return (false);
}

static bool is_spaces(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (true);
	return (false);
}

//The length of each token
size_t get_token_length(char *input_at_i)
{

    size_t i;
    
    char quotes;
    
      i = 0;

    // Check if the current character is an operator
    if (is_therechar(OPERATORS, input_at_i[i]))
    {
        // If the current character is an operator, check if it's a double-character operator example: && || >>
        if (input_at_i[i] == input_at_i[i + 1])
            // If it's a double-character operator, return a length of 2
            return (2);
        // If it's a single-character operator, return a length of 1
        return (1);
    }

    // Loop through the input string until we reach the end of the token
    while (input_at_i[i]
        &&!is_spaces(input_at_i[i])  // skip spaces
        &&!is_therechar(OPERATORS, input_at_i[i]))  // skip operators
    {
        // Check if the current character is a quote
        if (is_therechar(QUOTES, input_at_i[i]))
        {
            quotes = input_at_i[i++];
            // Skip over the quoted string
            while (input_at_i[i] && input_at_i[i]!= quotes)
            i++;
            break;
        }
        i++;
    }
    return (i);
}
