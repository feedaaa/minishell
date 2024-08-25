#include "../minishell.h"

int	cmd_unset(t_statement *s, t_vlst **head)
{
	int	i;

	i = 1;
	while (i < s->argc)
	{
		unset_var(s->argv[i], head);
		i += 1;
	}
	return (EXIT_SUCCESS);
}
