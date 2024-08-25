#include "minishell.h"

bool	unexpected_token(char token)
{
	ft_putstr_fd(UNEXPECTED_TOKEN, STDERR_FILENO);
	ft_putchar_fd(token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (true);
}

bool	has_operator(char *input)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (is_therechar(OPERATORS, input[i]))
			return (true);
		i += 1;
	}
	return (false);
}

// when operators like >, <, | etc., are used together improperly in a command
bool	invalid_syntax_on_operator(char *input)
{
	size_t	i;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	while (has_operator(&input[i]))
	{
		if (is_therechar(QUOTES, input[i]))
			in_quotes = !in_quotes;
		if (is_therechar(OPERATORS, input[i]) && !in_quotes)
		{
			if (input[i] == input[i + 1])
				i += 2;
			else
				i += 1;
			if (input[i] == ' ')
			{
				while (input[i] && input[i] == ' ')
					i += 1;
				if (is_therechar(OPERATORS, input[i]))
					return (unexpected_token(input[i]));
			}
			if (is_therechar(OPERATORS, input[i]))
				return (unexpected_token(input[i]));
		}
		i += 1;
	}
	return (false);
}

bool	invalid_syntax2(char *input)
{
	size_t	i;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	while (input[i])
	{
		if (is_therechar(QUOTES, input[i]))
			in_quotes = !in_quotes;
		if (((input[i] == '>' && input[i + 1] == '<') || (input[i] == '<'
					&& input[i + 1] == '>') || (input[i] == '|' && input[i
					+ 1] == '|')) && !in_quotes)
			return (unexpected_token(input[i + 1]));
		else if ((input[i] == '{' || input[i] == '}' || input[i] == '('
				|| input[i] == ')' || input[i] == '[' || input[i] == ']'
				|| input[i] == ';' || input[i] == '&' || input[i] == '*')
			&& !in_quotes)
			return (unexpected_token(input[i]));
		i += 1;
	}
	return (false);
}

bool	invalid_syntax(char *input)
{
	if (input[0] == '|')
		return (unexpected_token('|'));
	if (input[ft_strlen(input) - 1] == '|')
	{
		ft_putendl_fd(NO_PIPE_PROMPT, STDERR_FILENO);
		return (true);
	}
	if (is_therechar(REDIRECTS, input[ft_strlen(input) - 1]))
	{
		ft_putendl_fd(SYTX_ERR_RDR, STDERR_FILENO);
		return (true);
	}
	return (false);
}
