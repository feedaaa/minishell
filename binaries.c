#include "minishell.h"

//searching for its binary file
void	cmd_binaries(t_statement *statement, t_data *data)
{
	char	*bin_path;
	char	**paths;

	bin_path = statement->argv[0]; //first argument
	paths = get_paths(data->envp_lst); //checks where exactly the path is located
	if (is_absolute_path(statement)) //if its a path like /home/desktop
	{
		if (execve(bin_path, statement->argv, data->envp) == -1)  //listing the files in the directory
			exit_and_free_matrix(paths, statement->argv[0], 127);
		exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
		return ;
	}
	bin_path = get_bin_path(statement->argv[0], paths); //returns the binary path if its not an absloute path
	if (bin_path == NOT_FOUND)
	{
		cmd_not_found(statement->argv[0]);
		exit_and_free_matrix(paths, NULL, 127);
	}
	if (execve(bin_path, statement->argv, data->envp) == -1)
	{
		free(bin_path);
		exit_and_free_matrix(paths, statement->argv[0], 127);
	}
	free(bin_path);
	exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
}
void	cmd_not_found(char *cmd_name)
{
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

// path[0]: the first path
// path[1]: the second path
char	**get_paths(t_vlst *envp_lst)
{
	t_vlst	*temp;

	temp = envp_lst;
	while (!streq("PATH", temp->var_name)) //searches for the line where it has PATH name in the env
	{
		temp = temp->next;
		if (temp == NULL)
			return (NULL);
	}
	return (ft_split(temp->var_value, ':')); //it split the paths that are seperated by a :
}

//Returning the full path of the given command 
char	*get_bin_path(char *cmd, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*bin_path;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/"); 
		bin_path = join_free(tmp, cmd); //it joins the path with the command and searches for it 
		if (access(bin_path, F_OK | X_OK) == 0) //F_OK checks for existence, and X_OK checks for executable permission. If access returns 0, it means the file is found and executable,
			return (bin_path); //return the binary path 
		free(bin_path);  //if was not found free & check the next path
		i += 1;
	}
	return (NULL);
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

 bool	is_absolute_path(t_statement *statement)
{
	if (is_therechar(statement->argv[0], '/'))
		return (true);
	return (false);
}

char	*join_free(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*appended;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	appended = malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!appended)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len_s1)
		appended[i++] = s1[j++];
	j = 0;
	while (j < len_s2)
		appended[i++] = s2[j++];
	appended[i] = '\0';
	free(s1);
	return (appended);
}
