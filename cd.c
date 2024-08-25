#include "minishell.h"

static int	print_perror_msg(char *path)
{
	char	*error_msg;

	error_msg = ft_strjoin("minishell: cd: ", path);
	perror(error_msg);
	free(error_msg);
	return (EXIT_FAILURE);
}

static void	update_pwd(t_data *data)
{
	char	cwd[PATH_MAX];
	char	*updated_var;

	getcwd(cwd, PATH_MAX);
	updated_var = ft_strjoin("PWD=", cwd);
	save_user_vars(updated_var, &data->envp_lst, true);
	free(updated_var);
}

static void	update_oldpwd(char *temp, t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", temp);
	save_user_vars(oldpwd, &data->envp_lst, true);
	free(oldpwd);
}

static int	cd_oldpwd(char *temp, t_data *data)
{
	char	*oldpwd;

	oldpwd = get_fromvlst("OLDPWD", &data->envp_lst);
	if (!oldpwd)
	{
		ft_putendl_fd(OLDPWD_NOT_SET, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(oldpwd) == SUCCESS)
	{
		ft_putendl_fd(oldpwd, STDOUT_FILENO);
		update_oldpwd(&temp[0], data);
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	return (print_perror_msg(oldpwd));
}

int	cmd_cd(char *path, t_data *data)
{
	char	temp[PATH_MAX];

	getcwd(temp, PATH_MAX);               // takes you to the current directory
	if (path == NULL || streq(path, "~"))
		// Changes the directory to your home directory
	{
		update_oldpwd(&temp[0], data);
		chdir(getenv("HOME"));
			// switch from the current directory to the home directory
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	if (streq(path, "-")) // Switches to the previous directory you were in.
		return (cd_oldpwd(&temp[0], data));
	if (chdir(path) == SUCCESS)
	{
		update_oldpwd(&temp[0], data);
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	return (print_perror_msg(path)); // If chdir failed
}
