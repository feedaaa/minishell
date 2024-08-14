#include "minishell.h"

//this function to replace the name with the real value
char	*get_fromvlst(char *var_name, t_vlst **head)
{
	t_vlst	*temp;

	temp = *head;
	while (temp != NULL)
	{
		if (streq(var_name, temp->var_name))//check if var_name is the same as temp->var_name in this case it return the value of it
			return (temp->var_value);
		temp = temp->next;   //checks the next name that is saved in the env
	}
	return (NULL);
}

char	*get_varvalue_fromvlst(char *var_name, t_data *data)
{
	char	*var_value;

	var_value = get_fromvlst(var_name, &data->envp_lst);
	free(var_name);
	return (var_value);
}

size_t	exit_status_size(void)
{
	char	*exit_status;
	size_t	size;

	exit_status = ft_lltoa(g_exit_status);
	size = ft_strlen(exit_status);
	free(exit_status);
	return (size);
}
void	init_vars(size_t *i, size_t *size, bool *in_quotes, bool *in_dquotes)
{
	*i = 0;
	*size = 0;
	*in_quotes = false;
	*in_dquotes = false;
}

bool	single_dollar(char *input_at_i)
{
	return ((!input_at_i[1]
			|| input_at_i[1] == ' '
			|| input_at_i[1] == '\"'));
}

bool	streq(char *str1, char *str2)
{
	size_t	i;

	if ((str1 && !str2) || (!str1 && str2))
		return (false);
	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			return (false);
		i += 1;
	}
	return (true);
}

static long long	ft_digits(long long n)
{
	long long	digits;

	digits = 0;
	if (n <= 0)
		digits += 1;
	while (n != 0)
	{
		n /= 10;
		digits += 1;
	}
	return (digits);
}

char	*ft_lltoa(long long n)
{
	long long	digits;
	int			signal;
	char		*result;

	digits = ft_digits(n);
	signal = 1;
	result = malloc((digits + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[digits--] = '\0';
	if (n < 0)
	{
		signal = -1;
		result[0] = '-';
	}
	else if (n == 0)
		result[0] = '0';
	while (n != 0)
	{
		result[digits--] = (n % 10 * signal) + '0';
		n /= 10;
	}
	return (result);
}

