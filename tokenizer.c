#include "minishell.h"

char	**tokenize_input(char *input)
{
	size_t	i;
	size_t	k;
	char	**parsed;
	size_t	len;
	size_t	j;

	i = 0;
	k = 0;
	parsed = malloc((count_tokens(input) + 1) * sizeof(char *));
	while (input[i])
	{
		len = get_token_length(&input[i]);
		if (!len)
		{
			i += 1;
			continue ;
		}
		parsed[k] = malloc((len + 1) * sizeof(char));
		j = 0;
		while (input[i] && j < len)
			parsed[k][j++] = input[i++];
		parsed[k++][j] = '\0';
	}
	parsed[k] = NULL;
	return (parsed);
}

// Counts how many tokens do we have
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
		if (is_therechar(OPERATORS, *input)) // Counts the operators
			count += 1;
		if (is_therechar(QUOTES, *input) && *input == *(input + 1)) // In case there is a double quotation just move 2 steps to skip them
			input += 2;
		else if (is_therechar(QUOTES, *input))
			quotes = !quotes;
		if (*input != ' ' && !is_therechar(OPERATORS, *input) && !flag
			&& !quotes)
		{
			flag = true;
			count += 1;
		}
		else if (*input == ' ' || is_therechar(OPERATORS, *input)) // Counts nthe tokens that are seperated by spaces and operators
			flag = false;
		input += 1;
	}
	return (count);
}

// check if there str == c return true if not return false
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

bool	is_spaces(char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (true);
	return (false);
}

// The length of each token
size_t	get_token_length(char *input_at_i)
{
	size_t	i;
	char	quotes;

	i = 0;
	// Check if the current character is an operator
	if (is_therechar(OPERATORS, input_at_i[i]))
	{
		// If the current character is an operator,
		// check if it's a double-character operator example: && || >>
		if (input_at_i[i] == input_at_i[i + 1])
			// If it's a double-character operator, return a length of 2
			return (2);
		// If it's a single-character operator, return a length of 1
		return (1);
	}
	// Loop through the input string until we reach the end of the token
	while (input_at_i[i] && !is_spaces(input_at_i[i])  // skip spaces
			&& !is_therechar(OPERATORS, input_at_i[i])) // skip operators
	{
		// Check if the current character is a quote
		if (is_therechar(QUOTES, input_at_i[i]))
		{
			quotes = input_at_i[i++];
			// Skip over the quoted string
			while (input_at_i[i] && input_at_i[i] != quotes)
				i++;
			break ;
		}
		i++;
	}
	return (i);
}
