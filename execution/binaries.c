#include "../minishell.h"

void	cmd_binaries(t_statement *statement, t_data *data)
{
	char	*bin_path;
	char	**paths;

	bin_path = statement->argv[0];
	paths = get_paths(data->envp_lst);
	if (is_absolute_path(statement))
	{
		if (execve(bin_path, statement->argv, data->envp) == -1)
			exit_and_free_matrix(paths, statement->argv[0], 127);
		exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
		return ;
	}
	bin_path = get_bin_path(statement->argv[0], paths);
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

char	**get_paths(t_vlst *envp_lst)
{
	t_vlst	*temp;

	temp = envp_lst;
	while (!streq("PATH", temp->var_name))
	{
		temp = temp->next;
		if (temp == NULL)
			return (NULL);
	}
	return (ft_split(temp->var_value, ':'));
}

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
		bin_path = join_free(tmp, cmd);
		if (access(bin_path, F_OK | X_OK) == 0)
			return (bin_path);
		free(bin_path);
		i += 1;
	}
	return (NULL);
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
