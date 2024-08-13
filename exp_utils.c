
/*aysha header*/

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
