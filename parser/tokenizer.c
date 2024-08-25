#include "../minishell.h"

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
		if (is_therechar(OPERATORS, *input))
			count += 1;
		if (is_therechar(QUOTES, *input) && *input == *(input + 1))
			input += 2;
		else if (is_therechar(QUOTES, *input))
			quotes = !quotes;
		if (*input != ' ' && !is_therechar(OPERATORS, *input) && !flag
			&& !quotes)
		{
			flag = true;
			count += 1;
		}
		else if (*input == ' ' || is_therechar(OPERATORS, *input))
			flag = false;
		input += 1;
	}
	return (count);
}

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

size_t	get_token_length(char *input_at_i)
{
	size_t	i;
	char	quotes;

	i = 0;
	if (is_therechar(OPERATORS, input_at_i[i]))
	{
		if (input_at_i[i] == input_at_i[i + 1])
			return (2);
		return (1);
	}
	while (input_at_i[i] && !is_spaces(input_at_i[i])
		&& !is_therechar(OPERATORS, input_at_i[i]))
	{
		if (is_therechar(QUOTES, input_at_i[i]))
		{
			quotes = input_at_i[i++];
			while (input_at_i[i] && input_at_i[i] != quotes)
				i++;
			break ;
		}
		i++;
	}
	return (i);
}
