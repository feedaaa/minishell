#include "../minishell.h"

static int	cd_too_many_args(void)
{
	ft_putendl_fd(CD_TOO_MANY_ARGS, STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	call_cmd_echo(t_statement *s)
{
	t_statement	*temp;
	bool		has_n;
	size_t		j;

	j = 0;
	temp = s;
	has_n = false;
	if (s->argc > 2)
		has_n = is_therechar(s->argv[1], '-') && is_therechar(s->argv[1], 'n');
	cmd_echo(temp, has_n);
	temp = temp->next;
	while (temp != NULL && temp->argc > 2)
	{
		cmd_echo(temp, false);
		if (temp->operator == PIPE)
			break ;
		temp = temp->next;
	}
	if (!has_n)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	call_cmd_cd(t_statement *s, t_data *data)
{
	if (s->argc > 2)
		return (cd_too_many_args());
	else
		return (cmd_cd(s->argv[1], data));
}

int	call_cmd_unset(t_statement *s, t_data *data)
{
	if (s->argc == 1)
		return (EXIT_SUCCESS);
	return (cmd_unset(s, &data->envp_lst));
}
