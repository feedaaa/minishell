#include "../minishell.h"

void	clean_parsed(t_statement **statement_list, t_data *data)
{
	p_lstclear(statement_list);
	*statement_list = NULL;
	data->head = NULL;
}

void	exit_and_free_matrix(char **paths, char *cmd, int exit_status)
{
	if (cmd)
		perror(cmd);
	free_matrix(paths);
	if (!WIFSIGNALED(g_exit_status))
		g_exit_status = exit_status;
	exit(g_exit_status);
}
