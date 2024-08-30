#include "../minishell.h"

// bool	has_operator(char *input)
// {
// 	size_t	i;

// 	i = 0;
// 	while (input[i])
// 	{
// 		if (is_therechar(OPERATORS, input[i]))
// 			return (true);
// 		i += 1;
// 	}
// 	return (false);
// }

// bool	check_operator_syntax(char *input, size_t i)
// {
// 	if (input[i] == input[i + 1])
// 		i += 2;
// 	else
// 		i++;
// 	if (input[i] == ' ')
// 	{
// 		while (input[i] && input[i] == ' ')
// 			(i)++;
// 		if (is_therechar(OPERATORS, input[i]))
// 			return (unexpected_token(input[i]));
// 	}
// 	if (is_therechar(OPERATORS, input[i]))
// 		return (unexpected_token(input[i]));
// 	return (false);
// }

// static bool	handle_pipe_and_spaces(char *input, size_t *i)
// {
// 	(*i)++;
// 	while (input[*i] && input[*i] == ' ')
// 		(*i)++;
// 	if (is_therechar(REDIRECTS, input[*i]))
// 		return (false);
// 	return (true);
// }

// static bool	check_for_operator_syntax(char *input, size_t *i)
// {
// 	if (input[*i] == ' ')
// 	{
// 		while (input[*i] && input[*i] == ' ')
// 			(*i)++;
// 	}
// 	if (is_therechar(OPERATORS, input[*i]))
// 		return (unexpected_token(input[*i]));
// 	return (false);
// }

// bool	invalid_syntax_on_operator(char *input)
// {
// 	size_t	i;
// 	bool	in_quotes;

// 	i = 0;
// 	in_quotes = false;
// 	while (has_operator(&input[i]))
// 	{
// 		if (is_therechar(QUOTES, input[i]))
// 			in_quotes = !in_quotes;
// 		if (input[i] == input[i + 1])
// 			i += 2;
// 		if (is_therechar(OPERATORS, input[i]) && !in_quotes)
// 		{
// 			if (input[i] == '|')
// 			{
// 				if (!handle_pipe_and_spaces(input, &i))
// 					return (false);
// 			}
// 			else
// 				i += 1;
// 			if (check_for_operator_syntax(input, &i))
// 				return (true);
// 		}
// 		i += 1;
// 	}
// 	return (false);
// }

// bool	invalid_syntax2(char *input)
// {
// 	size_t	i;
// 	bool	in_quotes;

// 	i = 0;
// 	in_quotes = false;
// 	while (input[i])
// 	{
// 		if (is_therechar(QUOTES, input[i]))
// 			in_quotes = !in_quotes;
// 		if (((input[i] == '>' && input[i + 1] == '<') || (input[i] == '<'
// 					&& input[i + 1] == '>') || (input[i] == '|'
// 					&& input[i + 1] == '|')) && !in_quotes)
// 			return (unexpected_token(input[i + 1]));
// 		else if ((input[i] == '{' || input[i] == '}' || input[i] == '('
// 				|| input[i] == ')' || input[i] == '[' || input[i] == ']'
// 				|| input[i] == ';' || input[i] == '&' || input[i] == '*')
// 			&& !in_quotes)
// 			return (unexpected_token(input[i]));
// 		i += 1;
// 	}
// 	return (false);
// }

// bool	invalid_syntax(char *input)
// {
// 	if (input[0] == '|')
// 		return (unexpected_token('|'));
// 	if (input[ft_strlen(input) - 1] == '|')
// 	{
// 		ft_putendl_fd(NO_PIPE_PROMPT, STDERR_FILENO);
// 		return (true);
// 	}
// 	if (is_therechar(REDIRECTS, input[ft_strlen(input) - 1]))
// 	{
// 		ft_putendl_fd(SYTX_ERR_RDR, STDERR_FILENO);
// 		return (true);
// 	}
// 	return (false);
// }

#include "../minishell.h"

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

bool	check_operator_syntax(char *input, size_t i)
{
	if (input[i] == input[i + 1])
		i += 2;
	else
		i++;
	if (input[i] == ' ')
	{
		while (input[i] && input[i] == ' ')
			(i)++;
		if (is_therechar(OPERATORS, input[i]))
			return (unexpected_token(input[i]));
	}
	if (is_therechar(OPERATORS, input[i]))
		return (unexpected_token(input[i]));
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
					&& input[i + 1] == '>') || (input[i] == '|'
					&& input[i + 1] == '|')) && !in_quotes)
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
