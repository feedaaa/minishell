#include "minishell.h"

void	exec_type(t_statement *statement_list, t_data *data)
{
	int	temp_status = 0;

	if (p_lstsize(statement_list) == 1)
		// if it contains only one argumment of command
	{
		if (!builtin(statement_list, data) && fork() == 0)
			// check if its not a builtin command,
			// the child goes inside the loop
		{
			signal(SIGINT, child_signals);
				// changes depend on the child signals
			exec_executables(statement_list, data);
		}
	}
	else if (fork() == 0) // is it was more than one command
		exec_cmd(statement_list, data);
	waitpid(-1, &temp_status, 0);        
		// temp_status stores the status of the child process
	if (!WTERMSIG(temp_status))          
		// checks if the child process was not terminated by a signal
		g_exit_status = temp_status >> 8;
			// the 8th bit and afterwards contain the exit status of the child process
}

size_t	p_lstsize(t_statement *head)
{
	t_statement	*temp;
	size_t		size;

	temp = head;
	size = 0;
	while (temp != NULL)
	{
		temp = temp->next;
		size += 1;
	}
	return (size);
}

bool	builtin(t_statement *s, t_data *data)
{
	if (streq(s->argv[0], "exit"))
		cmd_exit(s, data);
	else if (streq(s->argv[0], "unset"))
		g_exit_status = call_cmd_unset(s, data);
	else if (streq(s->argv[0], "export"))
		g_exit_status = cmd_export(s, data);
	else if (streq(s->argv[0], "cd"))
		g_exit_status = call_cmd_cd(s, data);
	else if (ft_strchr(s->argv[0], '=') && is_valid_id(s->argv[0]))
		g_exit_status = save_user_vars(s->argv[0], &data->envp_lst, false);
	else if (streq(s->argv[0], "echo"))
		g_exit_status = call_cmd_echo(s);
	else if (streq(s->argv[0], "pwd"))
		g_exit_status = cmd_pwd();
	else if (streq(s->argv[0], "env"))
		g_exit_status = cmd_env(data);
	else
		return (false);
	return (true);
}

void	exec_executables(t_statement *node, t_data *data)
{
	if (builtin(node, data))
		return ;
	cmd_binaries(node, data);
}

void	exec_cmd(t_statement *current_node, t_data *data)
{
	signal(SIGINT, child_signals);
	if (current_node->operator== PIPE)
		exec_pipe(current_node, data);
	else if (current_node->operator== NONE)
		exec_executables(current_node, data);
	else
		exec_redirects(current_node, data);
	exit(g_exit_status);
}

void	child_signals(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_exit_status = 130;
		exit(130);
	}
}
