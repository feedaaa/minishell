/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compiled.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:29:50 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 11:10:43 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "libft/libft.h"

# define REDIRECTS "><"
# define OPERATORS "|<>"
# define QUOTES "\"\'"

# define NO_PIPE_PROMPT "minishell: no support for pipe prompt"
# define UNCLOSED_QUOTES "minishell: unclosed quotes"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token `"
# define SYTX_ERR_RDR "minishell: syntax error near unexpected token `newline'"

extern long long		g_exit_status;


typedef enum e_operator {
	NONE,                   //No specific operator.
	RDR_OUT_REPLACE,        //> in shells
	RDR_OUT_APPEND,        //>> in shells
	RDR_INPUT,             //< in shells
    RDR_INPUT_UNTIL,        //<< in shells
	PIPE,                  //| in shells
}t_operator;

//Lets assume NAME=VALUE so Name is saved in var_name and Value is saved in var_value
typedef struct s_vlst {
	char			*var_name;
	char			*var_value;
	bool			is_exported;
	struct s_vlst	*next;
}				t_vlst;

typedef struct s_statement {
    int                 argc; //The number of commands
    char              **argv; //The commnands between each operstor
    t_operator         operator;
	struct s_statement	*next;
} t_statement;

typedef struct s_data {
	char		**envp; //  Array of environment variables as strings  
	t_vlst		*envp_lst; //Pointer to the head of the linked list of environment variables
	t_statement	*head;
}				t_data;

//Main struct
typedef struct s_main
{
	t_vlst		*env;
	char		*input;
	t_statement	*tokens;
	char		**instructions;
	int			*in_fds;
	int			*out_fds;
	pid_t		*pids;
	int			cmd_count;
	int			exit_status;
	int			pipe_fds[2];
	int			prev_pipe_fd;
	int			open_fail;
}	t_main;

//array functions
void				init_oldpwd(t_vlst **head) ;
int				unset_var(char *var_name, t_vlst **head);
void				invalid_identifer(char *var_name);
int				save_user_vars(char *statement, t_vlst **head, bool to_export);
bool				get_exported_state(char *var_name, t_vlst **head);
int				unset_var(char *var_name, t_vlst **head);
void				v_lstadd_back(t_vlst **head, t_vlst *new);
t_vlst				*v_lstlast(t_vlst *node);
void				dismiss_signal(int signum);
void				config_signals(void);


//Parsing Functions
size_t				get_token_length(char *input_at_i);
char				**tokenize_input(char *input) ;
bool				is_therechar(const char *str, int ch);
size_t				count_tokens(char *input);
int					ft_strcmp(const char *s1, const char *s2);
t_operator			get_operator(char *operator);
t_statement			*parser(char *input);
size_t				remove_quotes_size(char	*parsed);
char				*remove_quotes(char	*parsed);
size_t				get_argc(char **parsed);
char				*ft_lltoa(long long n);
bool				unexpected_token(char token);
bool				has_operator(char *input);
bool				invalid_syntax_on_operator(char *input);
bool				invalid_syntax2(char *input);
bool				invalid_syntax(char *input);
void				destroy(t_data *data);
bool				valid_input(char *input, t_data *data);
int					unclosed_quotes(char *str);
void				free_matrix(char **matrix);
void				p_lstclear(t_statement **head);
void				v_lstclear(t_vlst **head);


//expanding function
size_t		expand_exit_status(char *expanded_input_at_i, size_t *i);
size_t		expand_variable(char *expanded_input_at_i, char *input,
	size_t *i, t_data *data);
size_t		expand_size(char *input_at_i, size_t *i, t_data *data);
int			expanded_size(char *input, t_data *data);
char		*expander(char *input, t_data *data);
char		*get_fromvlst(char *var_name, t_vlst **head);
char		*get_varvalue_fromvlst(char *var_name, t_data *data);
size_t		exit_status_size(void);
void		init_vars(size_t *i, size_t *size, bool *in_quotes, bool *in_dquotes);
bool		single_dollar(char *input_at_i);
bool		streq(char *str1, char *str2);


//setup functions
void		setup_shell(char **envp, t_data *data, t_statement **statement_list);
t_vlst		*init_envp_lst(char **envp);
char		**split_envp(char *env);
t_vlst		*v_new_node(char *var_name, char *var_value, bool is_exported);
t_statement	*p_new_node(int argc);
void		rl_replace_line(const char *s, int c);


//execution functions
void    	execution(t_statement *parsed_commands, char **env);
void 		execute_command(char *command, char **args, char **env);
int			is_builtin(char *command);
void		execute_builtin(t_statement *command);
void 		exec_one_cmd(t_statement *parsed_command, char **env);
void		execute_pipe(t_statement *parsed_commands, char **env);


//execution utils
char    	*ft_strcpy(char *dst, const char *src);
t_vlst		*msh_lstnew(char *var_name, char *var_value);
void		msh_lstadd_back(t_vlst **lst, t_vlst *new);
void		msh_lstdelone(t_vlst *lst, void (*del)(void*));

//clean-free function
void		clean_parsed(t_statement **statement_list, t_data *data);


//builtin functions
int			msh_pwd(t_statement *command);
void		msh_echo(t_statement *command);
void		msh_env(t_vlst *env);
void		msh_cd(t_statement *command);

//heredoc functions
int 		hd_child(t_statement *node, int fd);
int 		here_doc(t_statement *node);

#endif

//allowed functions
// readline, 
// rl_clear_history, 
// rl_on_new_line,
// rl_replace_line,
// rl_redisplay, 
// add_history,
// printf, 
// malloc, 
// free, 
// write, 
// access, 
// open, 
// read,
// close, 
// fork, 
// wait, 
// waitpid, 
// wait3, 
// wait4, 
// signal,
// sigaction, 
// sigemptyset, 
// sigaddset, 
// kill, 
// exit,
// getcwd, 
// chdir, 
// stat, 
// lstat, 
// fstat, 
// unlink, 
// execve,
// dup, 
// dup2, 
// pipe, 
// opendir, 
// readdir, 
// closedir,
// strerror, 
// perror, 
// isatty, 
// ttyname, 
// ttyslot, 
// ioctl,
// getenv, 
// tcsetattr, 
// tcgetattr, 
// tgetent, 
// tgetflag,
// tgetnum, 
// tgetstr, 
// tgoto, 
// tputs

#include "minishell.h"

void	clean_parsed(t_statement **statement_list, t_data *data)
{
	p_lstclear(statement_list);
	*statement_list = NULL;
	data->head = NULL;
}





#include "minishell.h"

void	dismiss_signal(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
}

//function to control the signals behavior
void	config_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &dismiss_signal;  //By setting it to &dismiss_signal, when the signal is SIGINT= ctrl C dismiss _signal function is applied
	sa.sa_flags = SA_RESTART;   //This prevents system calls from failing unexpectedly due to signal interruptions
	sigemptyset(&sa.sa_mask);  //initializes a signal set to be empty
	sigaddset(&sa.sa_mask, SIGINT); //adds the specified signal (SIGINT) to the signal set
	sigaction(SIGINT, &sa, NULL);  // Applies the configured signal action for SIGINT
	signal(SIGQUIT, SIG_IGN);   //Sets the action for SIGQUIT = 'ctrl /' to be ignored
}

//ctrl +D is not considered as a signal cause when you input something in the command line and click on ctrl+D nothing would happen so you will have to add it in the builtins


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:51:11 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:15:00 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strcpy(char *dst, const char *src)
{
    int i;
    
    i = 0;
    while(dst[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

t_vlst	*msh_lstnew(char *var_name, char *var_value)
{
	t_vlst	*new;

	new = malloc(sizeof(t_vlst));
	if (!new)
		return (NULL);
	new->var_name = ft_strdup(var_name);
	new->var_value = ft_strdup(var_value);
	new->next = NULL;
	return (new);
}

void	msh_lstadd_back(t_vlst **lst, t_vlst *new)
{
	t_vlst	*temp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = new;
}

void	msh_lstdelone(t_vlst *lst, void (*del)(void*))
{
	if (lst == NULL || del == NULL)
		return ;
	del(lst->var_name);
	del(lst->var_value);
	free(lst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:31:23 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 10:59:13 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_one_cmd(t_statement *parsed_command, char **env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        return (perror("fork()"));
    if (pid == 0)
    {
        execute_command(parsed_command->argv[0], parsed_command->argv, env);
        exit(EXIT_FAILURE);  // If exec fails
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
}

void execution(t_statement *parsed_commands, char **env)
{
    if (is_builtin(parsed_commands->argv[0]))
        execute_builtin(parsed_commands);

    if (parsed_commands->next != NULL)
    {
        execute_pipe(parsed_commands, env);
        execution(parsed_commands->next, env);
    }
    else
        exec_one_cmd(parsed_commands, env);
}


void execute_command(char *command, char **args, char **env)
{
    char *path = malloc(strlen(command) + 1);
    ft_strcpy(path, command);

    if (access(path, X_OK) == -1)
    {
        perror("access");
        free(path);
        exit(EXIT_FAILURE);
    }

    if (execve(path, args, env) == -1)
    {
        perror("execve");
        free(path);
        exit(EXIT_FAILURE);
    }
}

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




#include "minishell.h"

//update the expanded_input_at_i variable to the real value and count & returns the size of it
size_t	expand_exit_status(char *expanded_input_at_i, size_t *i)
{
	char	*exit_status;
	size_t	j;

	*i += 2;
	exit_status = ft_lltoa(g_exit_status);
	j = 0;
	while (exit_status[j])
	{
		expanded_input_at_i[j] = exit_status[j];
		j += 1;
	}
	free(exit_status);
	return (j);
}

// Skips the $ character.
// Determines the length of the variable name.
// Extracts the variable name and retrieves its value.
// Copies the variable value to the output string.
// Returns the size of the expanded variable.
////update the expanded_input_at_i variable to the real value and count & returns the size of it
size_t	expand_variable(char *expanded_input_at_i, char *input,
	size_t *i, t_data *data)
{
	char	*var_value;
	size_t	size;
	size_t	j;
	size_t	k;

	size = 0;
	j = 0;
	k = 0;
	*i += 1;
	if (!input[*i] || input[*i] == ' ' || input[*i] == '\"')
	{
		expanded_input_at_i[0] = '$';
		return (1);
	}
	while (input[*i + size] && input[*i + size] != ' '
		&& input[*i + size] != '\"' && !is_therechar(QUOTES, input[*i + size])
		&& input[*i + size] != '$')
		size += 1;
	var_value = get_varvalue_fromvlst(ft_substr(input, *i, size), data);
	*i += size;
	if (!var_value)
		return (0);
	while (var_value[k])
		expanded_input_at_i[j++] = var_value[k++];
	return (j);
}


//Goes through the environment of each variable to count how many characters
size_t	expand_size(char *input_at_i, size_t *i, t_data *data)
{
	size_t	var_size;
	char	*var_name;
	char	*var_value;

	*i += 1;
	if (single_dollar(input_at_i))
		return (1);
	var_size = 0;
	while (input_at_i[var_size + 1]
		&& input_at_i[var_size + 1] != ' '
		&& !is_therechar(QUOTES, input_at_i[var_size + 1])
		&& input_at_i[var_size + 1] != '$')
				var_size += 1;
	if (var_size == 0)
		return (0);
	var_name = ft_substr(input_at_i, 1, var_size);   //extract the name of a variable from the input string
	var_value = get_fromvlst(var_name, &data->envp_lst);  //returns the value of the variable name
	free(var_name);
	*i += var_size;  //update the index(i) to point to the inlout that is after the variable name
	if (!var_value)
		return (0);
	return (ft_strlen(var_value)); 
}


//it counts the size of the expanded part and returns the size
int	expanded_size(char *input, t_data *data)
{
	size_t	i;
	size_t	size;
	bool	in_quotes;
	bool	in_dquotes;

	init_vars(&i, &size, &in_quotes, &in_dquotes);
	while (input[i])
	{
		if (input[i] == '\"' && !in_quotes)
			in_dquotes = !in_dquotes;
		if (input[i] == '\'' && !in_dquotes)
			in_quotes = !in_quotes;
		if ((input[i] == '$' && input[i + 1] == '?') && !in_quotes)
		{
			size += exit_status_size() - 1; //exit_status_size(): this function will count the expanded part for ? which is the exit status and will subtract 1 to not count $ 
			i += 1;
		}
		else if (input[i] == '$' && !in_quotes)
			size += expand_size(&(input[i]), &i, data) - 1;
		else
			i += 1;
		size += 1;
	}
	return (size);
}


//The main function for the expanding part and took input & data as parameters to connect the environment of each input(Name) given
char	*expander(char *input, t_data *data)
{
	size_t	i;
	size_t	j;
	bool	in_quotes;
	bool	in_dquotes;
	char	*expanded_input;

	init_vars(&i, &j, &in_quotes, &in_dquotes); //initialize the given variables as parameters
	expanded_input = malloc((expanded_size(input, data) + 1) * sizeof(char));
	while (input[i])
	{
		if (input[i] == '\"' && !in_quotes)
			in_dquotes = !in_dquotes;
		if (input[i] == '\'' && !in_dquotes)
			in_quotes = !in_quotes;
		if (input[i] == '$' && input[i + 1] == '?' && !in_quotes)
			j += expand_exit_status(&(expanded_input[j]), &i);
		else if (input[i] && input[i] == '$' && !in_quotes)
			j += expand_variable(&(expanded_input[j]), input, &i, data);
		else
			expanded_input[j++] = input[i++];
	}
	expanded_input[j] = '\0';
	free(input);
	return (expanded_input);
}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:15 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 10:40:17 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int here_doc(t_statement *node)
{
    pid_t pid;
    int fd;
    char *tmp_file;
    
    tmp_file = "/tmp/heredoc_tmp";
    fd = open(tmp_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (perror("open: "), -1);
	hd_child(node, fd);
    fd = open(tmp_file, O_RDONLY);
    if (fd == -1)
        return(perror("open: "), -1);
    pid = fork();
    if (pid == -1)
        return(perror("fork"), close(fd));
    if (pid == 0)
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
		execve("/bin/cat", node->argv, NULL);
    }
    close(fd);
	return (0);
}

int hd_child(t_statement *node, int fd)
{
    char *line;
	size_t len;
    char *delimiter;
	
	len = 0;
	line = NULL;
    delimiter = node->next->argv[1];
	while (1)
    {
        printf("> ");
        get_next_line(fd);
        if(!&get_next_line)
        {
            perror("get_next_line");
            return(free(line), close(fd), -1);
        }
        if (ft_strcmp(line, delimiter) == 0)
            break;
        write(fd, line, ft_strlen(line));
        free(line);
        line = NULL;
        len = 0;
    }
    free(line);
	return (close(fd), 0);
}


#include "minishell.h"


int	unclosed_quotes(char *str)
{
	char	last_opened;

	last_opened = 0;
	while (*str && !last_opened)
	{
		if (*str == '\'' || *str == '"')
			last_opened = *str;
		str++;
	}
	while (*str && last_opened)
	{
		if (*str && *str == last_opened)
			last_opened = 0;
		str++;
	}
	if (*str)
		return (unclosed_quotes(str));
	else if (!last_opened)
		return (0);
	else
		return (1);
}

void	free_matrix(char **matrix)
{
	size_t	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i += 1;
	}
	free(matrix);
	matrix = NULL;
}

void	p_lstclear(t_statement **head)
{
	t_statement	*temp;
	t_statement	*next_node;

	if (!head)
		return ;
	temp = *head;
	while (temp != NULL)
	{
		next_node = temp->next;
		free_matrix(temp->argv);
		free(temp);
		temp = next_node;
	}
	*head = NULL;
}


void	v_lstclear(t_vlst **head)
{
	t_vlst	*temp;
	t_vlst	*next_node;

	if (!head)
		return ;
	temp = *head;
	while (temp != NULL)
	{
		next_node = temp->next;
		free(temp->var_name);
		free(temp->var_value);
		free(temp);
		temp = next_node;
	}
	*head = NULL;
}


#include "minishell.h"

bool	unexpected_token(char token)
{
	ft_putstr_fd(UNEXPECTED_TOKEN, STDERR_FILENO);
	ft_putchar_fd(token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (true);
}

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

bool	invalid_syntax_on_operator(char *input)
{
	size_t	i;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	while (has_operator(&input[i]))
	{
		if (is_therechar(QUOTES, input[i]))
			in_quotes = !in_quotes;
		if (is_therechar(OPERATORS, input[i]) && !in_quotes)
		{
			if (input[i] == input[i + 1])
				i += 2;
			else
				i += 1;
			if (input[i] == ' ')
			{
				while (input[i] && input[i] == ' ')
					i += 1;
				if (is_therechar(OPERATORS, input[i]))
					return (unexpected_token(input[i]));
			}
			if (is_therechar(OPERATORS, input[i]))
				return (unexpected_token(input[i]));
		}
		i += 1;
	}
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
		if (((input[i] == '>' && input[i + 1] == '<')
				|| (input[i] == '<' && input[i + 1] == '>')
				|| (input[i] == '|' && input[i + 1] == '|')) && !in_quotes)
			return (unexpected_token(input[i + 1]));
		else if ((input[i] == '{' || input[i] == '}'
				|| input[i] == '(' || input[i] == ')'
				|| input[i] == '[' || input[i] == ']'
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:08:36 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 18:41:46 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
long long	g_exit_status = 0;

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i += 1;
	}
	if (src[i] && i < n)
	{
		while (i < n)
			dest[i++] = '\0';
	}
	dest[i] = '\0';
	return (dest);
}

static char	*trim_free(char *s1, char const *set)
{
	size_t	beg;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	beg = 0;
	while (s1[beg] != '\0' && ft_strchr(set, s1[beg]) != NULL)
		beg += 1;
	end = ft_strlen(s1 + beg);
	while (end > beg && ft_strchr(set, s1[(beg + end) - 1]) != NULL)
		end -= 1;
	trimmed_str = malloc((end + 1) * sizeof(char));
	if (!trimmed_str)
		return (NULL);
	ft_strncpy(trimmed_str, (s1 + beg), end);
	free(s1);
	return (trimmed_str);
}

static char	*get_input(void)
{
	char	*raw_input;
	char	*input;

	raw_input = readline("minishell$ ");
	input = trim_free(raw_input, " \t");
	return (input);
}

void	print_tokens(t_statement *list)
{
	t_statement	*temp = list;

	while (temp)
	{
		int i = -1;
		if (temp->argv)
		{
			while (temp->argv[++i])
				printf("%s ", temp->argv[i]);
			printf("\n");
		}
		temp = temp->next;
	}
}

int main(int ac, char **av, char **env)
{
	char *command;
	(void)av;
	(void)ac;
	t_data		data;
	t_statement  *parsed_commands;
	int size;
	size = 0;
	setup_shell(env, &data, &parsed_commands);
	while (true)
	{
		command = get_input();
		if (!valid_input(command, &data))    //checks if there are any syntax errors
			continue ;
		add_history(command);
		command = expander(command, &data);
    if (!command[0])
		{
			free(command);
			continue ;
		}
    parsed_commands = parser(command);
	print_tokens(parsed_commands);
		execution(parsed_commands, env);
		// execute_command(av[1], env); //to do
		clean_parsed(&parsed_commands, &data); //to clear/free everything and handle the next command 
	}
	return (0);
}



#include "minishell.h"

void	init_oldpwd(t_vlst **head)     //to create OLDPWD for 'cd -' command.. it saves the old directory on OLDPWD env variable
{
	char	*temp;

	unset_var("OLDPWD", head);
	temp = ft_strjoin("OLDPWD=", getenv("HOME"));
	save_user_vars(temp, head, true);
	free(temp);
}

int	unset_var(char *var_name, t_vlst **head)
{
	t_vlst	*temp;
	t_vlst	*next_node;

	temp = *head;
	if (ft_strchr(var_name, '='))
	{
		invalid_identifer(var_name);
		return (EXIT_FAILURE);
	}
	while (temp && temp->next != NULL)
	{
		if (streq(var_name, temp->next->var_name))  //checks if the environmnet variable and the next env variable are the same
		{
			next_node = temp->next->next;
			free(temp->next->var_name);     //delete the next var-name
			free(temp->next->var_value);   ////delete the next var-value
			free(temp->next);
			temp->next = next_node;       //connect to the one after the next node
			break ;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

//This function is responsible for printing an error message when an invalid environment variable name is encountered.
void	invalid_identifer(char *var_name)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(var_name, STDERR_FILENO);
	ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
}

int	save_user_vars(char *statement, t_vlst **head, bool to_export)
{
	char	**line;

	line = split_envp(statement);// line[0]= Variable line[1]=value
	if (get_exported_state(line[0], head) && !to_export) //this function get_exported_state checks if this Variable is exported or not
		to_export = true;
	unset_var(line[0], head);  //it needs to be deleted to be updated
	v_lstadd_back(head, v_new_node(line[0], line[1], to_export));
	free(line);
	return (EXIT_SUCCESS);
}

//checks if the env exported or not
bool	get_exported_state(char *var_name, t_vlst **head)
{
	t_vlst	*temp;

	temp = *head;
	while (temp != NULL)
	{
		if (streq(var_name, temp->var_name))
			return (temp->is_exported);
		temp = temp->next;
	}
	return (false);
}


void	v_lstadd_back(t_vlst **head, t_vlst *new)
{
	t_vlst	*temp;

	if (head)
	{
		if (!*head)
			*head = new;
		else
		{
			temp = v_lstlast(*(head));
			temp->next = new;
		}
	}
}

t_vlst	*v_lstlast(t_vlst *node)
{
	while (node)
	{
		if (!node->next)
			break ;
		node = node->next;
	}
	return (node);
}



//Still need to do the function that seperates the commands and the operator
#include "minishell.h"

t_statement			*parser(char *input)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
    size_t      j;

	parsed = tokenize_input(input);
	// free(input);
	temp = p_new_node(get_argc(&parsed[0]));//create a new node for the first argument
	head = temp;
	i = 0;
	while (parsed[i])//goes through the first token
	{
		j = 0;
		while (parsed[i] && !is_therechar(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		temp->argv[j] = NULL;   //to know its the end or there is an operator after
		if (!parsed[i])
			break ;
		temp->operator = get_operator(parsed[i++]);
		temp->next = p_new_node(get_argc(&parsed[j]));// creatinmg a node to connect the rest of the parsed part
		temp = temp->next;
	}
	temp->next = NULL;
	// free(parsed);
	return (head);
}

t_statement	*p_new_node(int argc)
{
	t_statement	*new_node;

	new_node = malloc(sizeof(t_statement));
	new_node->argc = argc;
	new_node->argv = malloc((argc + 1) * sizeof(char *));
	new_node->operator = NONE;
	new_node->next = NULL;
	return (new_node);
}



#include "minishell.h"

//represents the number of command-line arguments passed to the program.
size_t	get_argc(char **parsed)
{
	size_t	i;

	i = 0;
	while (parsed[i])
		i += 1;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_operator	get_operator(char *operator)
{
	t_operator	op;

	if (!operator)
		op = NONE;
	else if (!ft_strcmp(operator, "|"))
		op = PIPE;
	else if (!ft_strcmp(operator, ">>"))
		op = RDR_OUT_APPEND;
	else if (!ft_strcmp(operator, ">"))
		op = RDR_OUT_REPLACE;
	else if (!ft_strcmp(operator, "<<"))
		op = RDR_INPUT_UNTIL;
	else if (!ft_strcmp(operator, "<"))
		op = RDR_INPUT;
	else
		op = NONE;
	free(operator);
	return (op);
}


size_t	remove_quotes_size(char	*parsed)
{
	size_t	i;
	size_t	size;
	char	quotes;

	i = 0;
	size = 0;
	while (parsed[i])
	{
		while (parsed[i] && !is_therechar(QUOTES, parsed[i]))
		{
			i += 1;
			size += 1;
		}
		if (!parsed[i])
			break ;
		quotes = parsed[i++];
		while (parsed[i] && parsed[i] != quotes)
		{
			i += 1;
			size += 1;
		}
		quotes = 0;
	}
	return (size);
}

char	*remove_quotes(char	*parsed)
{
	size_t	i;
	size_t	j;
	char	quotes;
	char	*unquoted_parsed;

	i = 0;
	j = 0;
	quotes = '\0';
	unquoted_parsed = malloc((remove_quotes_size(parsed) + 1) * sizeof(char));
	while (parsed[i])
	{
		while (parsed[i] && !is_therechar(QUOTES, parsed[i]))
			unquoted_parsed[j++] = parsed[i++];
		if (!parsed[i])
			break ;
		quotes = parsed[i];
		i += 1;
		while (parsed[i] && parsed[i] != quotes)
			unquoted_parsed[j++] = parsed[i++];
		quotes = '\0';
	}
	unquoted_parsed[j] = '\0';
	free(parsed);
	return (unquoted_parsed);
}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:33:25 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 10:59:28 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipe(t_statement *parsed_commands, char **env)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return (perror("pipe()"));
    pid_t pid = fork();
    if (pid == -1)
        return (perror("fork()"));
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_command(parsed_commands->argv[0], parsed_commands->argv, env);
        exit(EXIT_FAILURE);  // If exec fails
    }
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}




/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:18:05 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:18:06 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

//function to caonnect as a linked list
t_vlst	*v_new_node(char *var_name, char *var_value, bool is_exported)
{
	t_vlst	*new_node;

	new_node = malloc(sizeof(t_vlst));
	new_node->var_name = var_name;
	new_node->var_value = var_value;
	new_node->is_exported = is_exported;
	new_node->next = NULL;
	return (new_node);
}

// This function splits an environment variable string into its name and value components,
//storing them in a dynamically allocated array of strings.

char	**split_envp(char *env)
{
	char	**splitted;
	size_t	indexof_equal;


	splitted = malloc(3 * sizeof(char *));
		// 3 because we have the 1-name variable and 2-the expanded value and 3-the null
	if (!splitted)
		return (NULL);
	indexof_equal = ft_strchr(env, '=') - env;
	splitted[0] = ft_substr(env, 0, indexof_equal); //"NAME"starting from 0 to where there was equal sign
	splitted[1] = ft_substr(env, indexof_equal + 1, ft_strlen(&env[indexof_equal])); //"VALUE" starting from the equal sign and afterwards
	splitted[2] = NULL; // To indicate the end of the first environment variable
	return (splitted);
}

// initializes a linked list of environment variables from an array of environment strings
t_vlst	*init_envp_lst(char **envp)
{
	t_vlst	*head;
	t_vlst	*temp;
	char	**line;
	size_t	i;

	line = split_envp(envp[0]);
		// splits the first environment variable into array of name ane the other array is the value if we assume NAME=VALUE
	head = v_new_node(line[0], line[1], true);
	free(line);
	i = 1;
	temp = head;
	while (envp[i])
	{
		line = split_envp(envp[i]);
		temp->next = v_new_node(line[0], line[1], true);
		free(line);
		temp = temp->next;
		i += 1;
	}
	init_oldpwd(&head);
	return (head);
}

// initialising the shell based on the environment
void	setup_shell(char **envp, t_data *data, t_statement **statement_list)
{
	data->envp = envp;
	data->envp_lst = init_envp_lst(envp);
	data->head = NULL;
	*statement_list = NULL;
	// config_signals(); //this function to update the shell depending on some signals such as ctrl+c ...
}



#include "minishell.h"

char	**tokenize_input(char *input) 
{
	size_t		i;
	size_t		k;
	char		**parsed;
	size_t		len;
	size_t		j;

	i = 0;
	k = 0;
	parsed = malloc((count_tokens(input) + 1) * sizeof(char *));
	while (input[i])
	{
		len =  get_token_length(&input[i]);
		if (!len)
		{
			i += 1;
			continue ;
		}
		parsed[k] = malloc((len + 1) * sizeof(char));
		j = 0;
		while (input[i] && j < len)
			parsed[k][j++] = input[i++];
		parsed[k++][j] = '\0';
	}
	parsed[k] = NULL;
	return (parsed);
}

// Counts how many tokens do we have
size_t	count_tokens(char *input)
{
	size_t	count;
	bool	flag;
	bool	quotes;

	count = 0;
	flag = false;
	quotes = false;
	while (*input)
	{
		if (is_therechar(OPERATORS, *input)) // Counts the operators
			count += 1;
		if (is_therechar(QUOTES, *input) && *input == *(input + 1))
			// In case there is a double quotation just move 2 steps to skip them
			input += 2;
		else if (is_therechar(QUOTES, *input))
			quotes = !quotes;
		if (*input != ' ' && !is_therechar(OPERATORS, *input) && !flag
			&& !quotes)
		{
			flag = true;
			count += 1;
		}
		else if (*input == ' ' || is_therechar(OPERATORS, *input))
			// Counts nthe tokens that are seperated by spaces and operators
			flag = false;
		input += 1;
	}
	return (count);
}

// check if there str == c return true if not return false
bool	is_therechar(const char *str, int ch)
{
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ch)
			return (true);
		i++;
	}
	return (false);
}

static bool	is_spaces(char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (true);
	return (false);
}

// The length of each token
size_t	get_token_length(char *input_at_i)
{
	size_t	i;
	char	quotes;

	i = 0;
	// Check if the current character is an operator
	if (is_therechar(OPERATORS, input_at_i[i]))
	{
		// If the current character is an operator,
		//check if it's a double-character operator example: && || >>
		if (input_at_i[i] == input_at_i[i + 1]) // If it's a double-character operator, return a length of 2
			return (2);
		// If it's a single-character operator, return a length of 1
		return (1);
	}
	// Loop through the input string until we reach the end of the token
	while (input_at_i[i] && !is_spaces(input_at_i[i])  // skip spaces
			&& !is_therechar(OPERATORS, input_at_i[i])) // skip operators
	{
		// Check if the current character is a quote
		if (is_therechar(QUOTES, input_at_i[i]))
		{
			quotes = input_at_i[i++];
			// Skip over the quoted string
			while (input_at_i[i] && input_at_i[i] != quotes)
				i++;
			break ;
		}
		i++;
	}
	return (i);
}


#include "minishell.h"

void	destroy(t_data *data)
{
	// rl_clear_history();
	if (data->envp_lst)
		v_lstclear(&data->envp_lst);
	if (data->head)
		p_lstclear(&data->head);
}

static void	exit_shell(int exit_status, t_data *data)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	g_exit_status = exit_status;
	if (data)
		destroy(data);
	exit(g_exit_status);
}

bool	valid_input(char *input, t_data *data)
{
	bool	valid;

	valid = true;
	if (input == NULL)
	{
		free(input);
		exit_shell(EXIT_SUCCESS, data);
	}
	if (input[0] == '\0')
		valid = false;
	else if (unclosed_quotes(input))
	{
		ft_putendl_fd(UNCLOSED_QUOTES, STDERR_FILENO);
		valid = false;
	}
	else if (invalid_syntax(input)
		|| invalid_syntax2(input)
		|| invalid_syntax_on_operator(input))
		valid = false;
	if (!valid)
	{
		free(input);
		g_exit_status = 2;
	}
	return (valid);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:38:40 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:16:35 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *command)
{
	if (!ft_strncmp(command, "exit ", 5)
		|| !ft_strncmp(command, "env ", 4)
		|| !ft_strncmp(command, "export ", 7)
		|| !ft_strncmp(command, "echo ", 5)
		|| !ft_strncmp(command, "pwd ", 4)
		|| !ft_strncmp(command, "cd ", 3)
		|| !ft_strncmp(command, "unset", 6))
		return (1);
	return (0);
}

void execute_builtin(t_statement *command)
{
    if (strcmp(command->argv[0], "cd") == 0)
        msh_cd(command);
    else if (strcmp(command->argv[0], "pwd") == 0)
        msh_pwd(command);
    else if (strcmp(command->argv[0], "echo") == 0)
        msh_echo(command);
    // else if (strcmp(command->argv[0], "env") == 0)
    //     msh_env();
    // else if (strcmp(command->argv[0], "export") == 0)
    //     msh_export(command);
    // else if (strcmp(command->argv[0], "unset") == 0)
    //     msh_unset(command);
    // else if(strcmp(command->argv[0], "exit") == 0)
    //     msh_exit(command);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:07:38 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 12:41:48 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void msh_cd(t_statement *command)
{
    char *path;
    char **env;

    if (!command->argv[1])
    {
        path = getenv("HOME");
        if (!path)
            perror("getenv");
    }
    else
        path = command->argv[1];

    if (chdir(path) == -1)
        perror("chdir");
    else
    {
        char cwd[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        setenv("PWD", cwd, 1);
        for (int i = 0; env[i]; i++)
        {
            if (strcmp(env[i], "PWD=*") == 0)
            {
                free(env[i]);
                env[i] = malloc(strlen("PWD=") + strlen(cwd) + 1);
                sprintf(env[i], "PWD=%s", cwd);
                break;
            }
        }
    }
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:41:16 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 11:50:39 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void msh_echo(t_statement *command)
{
    int i = 1;
    int n = 0;

    while (command->argv[i] != NULL)
    {
        if (strcmp(command->argv[i], "-n") == 0)
            n = 1;
        else
        {
            ft_putstr_fd(command->argv[i], STDOUT_FILENO);
            if (command->argv[i + 1] != NULL)
                ft_putstr_fd(" ", STDOUT_FILENO);
        }
        i++;
    }
    if (!n)
        ft_putstr_fd("\n", STDOUT_FILENO);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:52:57 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 16:02:25 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	msh_env(t_vlst *env)
{
	t_vlst	*envp;

	envp = env;
	while (envp)
	{
		printf("%s=%s\n", envp->var_name, envp->var_value);
		envp = envp->next;
	}
}

char	*msh_getenv(t_vlst *env, const char *var_name)
{
	t_vlst	*envp;

	envp = env;
	while (envp)
	{
		if (ft_strcmp(envp->var_name, var_name) == 0)
			return (envp->var_value);
		envp = envp->next;
	}
	return (NULL);
}

void    msh_setenv(t_vlst **env, const char *var_name, const char *var_value)
{
    t_vlst    *envp;

    envp = *env;
    while (envp)
    {
        if (ft_strcmp(envp->var_name, var_name) == 0)
        {
            free(envp->var_value);
            envp->var_value = ft_strdup(var_value);
            return ;
        }
        envp = envp->next;
    }
    envp = msh_lstnew(ft_strdup(var_name), ft_strdup(envp->var_value));
    msh_lstadd_back(env, envp);
}

void	msh_unsetenv(t_vlst **env, const char *var_name)
{
	t_vlst	*envp;
	t_vlst	*prev;

	envp = *env;
	prev = NULL;
	while (envp)
	{
		if (ft_strcmp(envp->var_name, var_name) == 0)
		{
			if (prev)
				prev->next = envp->next;
			else
				*env = envp->next;
			msh_lstdelone(envp, free);
			return ;
		}
		prev = envp;
		envp = envp->next;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:38:01 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 11:47:54 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int msh_pwd(t_statement *command)
{
	(void)command;
	char cwd[1024];
	
	if(getcwd(cwd, 1024))
	{
		ft_putendl_fd(cwd, 0);
		return (0);
	}
	else
	{
		ft_putstr_fd("msh: pwd: ", 2);
		ft_putstr_fd("minishell: pwd error", 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:27:21 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/21 14:28:05 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	msh_unsetenv(const char *name)
{
	extern char **environ;
	char **envp;
	char *var_name;
	size_t len;

	len = ft_strlen(name);
	envp = environ;
	while (*envp)
	{
		var_name = ft_substr(*envp, 0, ft_strchr(*envp, '=') - *envp);
		if (ft_strcmp(var_name, name) == 0)
		{
			free(var_name);
			*envp = NULL;
			return ;
		}
		free(var_name);
		envp++;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:30:26 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:21 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return (result * sign);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 14:21:16 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:29 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	ft_memset(s, 0, len);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:51:09 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:33 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*s;

	if (count != 0 && size != 0 && (count > 2147483647 / size))
		return (NULL);
	s = malloc(count * size);
	if (!s)
		return (NULL);
	ft_bzero(s, size * count);
	return (s);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:45:05 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:42 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((ft_isalpha(c) == 1)
		|| (ft_isdigit(c) == 1))
		return (1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 09:38:16 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:46 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:40:01 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:50 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:35:56 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:56 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:51:33 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:45:59 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c >= 32 && c < 127)
		return (1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 13:40:48 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:04 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	getsize(int n)
{
	size_t	len;
	long	num;

	num = n;
	len = 0;
	if (num == 0)
	{
		len = 1;
		return (len);
	}
	if (num < 0)
	{
		num *= -1;
		len++;
	}
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*str;
	size_t		i;
	long int	num;

	num = n;
	i = getsize(n);
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i--] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		str[i] = 48 + (num % 10);
		num /= 10;
		i--;
	}
	return (str);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:43:28 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:08 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lastone;

	if (lst)
	{
		if (*lst)
		{
			lastone = ft_lstlast(*lst);
			lastone->next = new;
		}
		else
			*lst = new;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:42:28 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:13 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst)
	{
		if (*lst)
			new->next = *lst;
		*lst = new;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:44:30 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:16 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void	*))
{
	t_list	*temp;

	if (lst)
	{
		while (lst && *lst)
		{
			temp = (*lst)->next;
			ft_lstdelone(*lst, del);
			*lst = temp;
		}
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:44:16 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:20 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (del != NULL)
	{
		if (lst)
		{
			(*del)(lst->content);
			free(lst);
		}
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:44:51 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:24 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (f != NULL)
	{
		while (lst)
		{
			(*f)(lst->content);
			lst = lst->next;
		}
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:43:12 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:28 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:58:03 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:31 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*temp;
	void	*cont;

	if (!lst || !del || !f)
		return (NULL);
	newlst = (NULL);
	while (lst)
	{
		cont = f(lst->content);
		temp = ft_lstnew(cont);
		if (!temp)
		{
			ft_lstclear(&newlst, del);
			(*del)(cont);
			return (NULL);
		}
		ft_lstadd_back(&newlst, temp);
		lst = lst->next;
	}
	return (newlst);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:41:57 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:35 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:42:46 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:39 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 18:21:34 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:43 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return (str + i);
		i++;
	}
	return (NULL);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 18:56:24 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:47 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*str1;
	unsigned char		*str2;
	size_t				i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:48:05 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:53 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*dest2;
	const char	*src2;
	size_t		i;

	dest2 = (char *)dest;
	src2 = (const char *)src;
	if (n == 0 || dest == src)
		return (dest);
	i = 0;
	while (i < n)
	{
		dest2[i] = src2[i];
		i++;
	}
	return (dest);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:34:23 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:46:57 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*dst2;
	const char	*src2;

	i = 0;
	dst2 = (char *)dst;
	src2 = (const char *)src;
	if (dst2 < src2)
	{
		while (i < len)
		{
			dst2[i] = src2[i];
			i++;
		}
	}
	else if (dst2 > src2)
	{
		i = len;
		while (i > 0)
		{
			dst2[i - 1] = src2[i - 1];
			i--;
		}
	}
	return (dst);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:43:42 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:01 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*set;

	set = (char *)b;
	i = 0;
	while (i < len)
		set[i++] = c;
	return (b);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 22:08:28 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:05 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 22:35:37 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:08 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write (fd, &s[i], 1);
		i++;
	}
	write (fd, "\n", 1);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 22:10:20 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:13 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write (fd, "-2147483648", 11);
	}
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(n * -1, fd);
	}
	else if (n <= 9)
	{
		ft_putchar_fd(n + '0', fd);
	}
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 22:14:32 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:16 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write (fd, &s[i], 1);
		i++;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 17:19:13 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:20 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**freeer(char **split, int i)
{
	while (i-- > 0)
		free (split[i]);
	free (split);
	return (NULL);
}

static int	words(const char *s, char c)
{
	int		wordcount;

	wordcount = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		while (*s != c && *s)
		{
			s++;
			if (*s == c || *s == '\0')
				wordcount++;
		}
	}
	return (wordcount);
}

static char	*wordsize(const char *s, char c)
{
	int		i;
	char	*word;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\0' && s[i] != c)
		i++;
	word = malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] != c)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	t_split	var;

	var.i = 0;
	if (!s)
		return (NULL);
	var.wordcount = words(s, c);
	var.split = malloc(sizeof(char *) * (var.wordcount + 1));
	if (!var.split)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			var.split[var.i] = wordsize(s, c);
			if (!var.split[var.i])
				return (freeer(var.split, var.i));
			var.i++;
			while (*s && *s != c)
				s++;
		}
	}
	var.split[var.i] = (NULL);
	return (var.split);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:07:56 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:25 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	if ((char)c == '\0')
		return ((char *)s + ft_strlen(s));
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:58:26 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:29 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*s2;

	i = 0;
	if (!s1)
		return (NULL);
	s2 = (char *)malloc((ft_strlen(s1) * sizeof(char)) + 1);
	if (!s2)
		return (NULL);
	while (s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 22:11:38 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:35 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 20:30:05 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:49:57 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*new;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	i = 0;
	j = 0;
	new = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
		new[i++] = s2[j++];
	new[i++] = '\0';
	return (new);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:47 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:43 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	s;
	size_t	t;
	size_t	destl;
	size_t	srcl;

	s = 0;
	t = 0;
	while (dest[t] != '\0')
		t++;
	destl = t;
	srcl = ft_strlen(src);
	if (dstsize == 0 || dstsize <= destl)
		return (srcl + dstsize);
	while (src[s] != '\0' && s < dstsize - destl - 1)
		dest[t++] = src[s++];
	dest[t] = '\0';
	return (destl + srcl);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:51:22 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:47 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	x;

	x = ft_strlen(src);
	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && (i < size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (x);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:17:48 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:51 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 22:23:38 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:47:55 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*new;
	int		i;
	int		len;

	if (!s || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	new = (char *)malloc(len + 1);
	if (!new)
		return (NULL);
	while (s[i] != '\0')
	{
		new[i] = f(i, s[i]);
		i++;
	}
	new[i] = '\0';
	return (new);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:25:31 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:00 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	if (!s1 || !s2)
		return (-2);
	while ((str1[i] != '\0' || str2[i] != '\0') && i < n)
	{
		if (str1[i] > str2[i])
			return (1);
		else if (str1[i] < str2[i])
			return (-1);
		i++;
	}
	if (str1[i] > str2[i])
		return (1);
	else if (str1[i] < str2[i])
		return (-1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:54:10 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:04 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (needle[j] == '\0')
		return ((char *)hay);
	while ((hay[i] != '\0') && (i < len))
	{
		while (hay[i + j] != '\0' && hay[i + j] == needle[j] && i + j < len)
			j++;
		if (needle[j] == '\0')
			return ((char *)hay + i);
		j = 0;
		i++;
	}
	return (NULL);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:08:44 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:08 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t		i;

	i = ft_strlen(s);
	while ((int)i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i--;
	}
	if ((char)c == '\0')
		return ((char *)s + i);
	return (NULL);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:47:38 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:13 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checkset(char s, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (s == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char *s1, char const *set, int flag)
{
	char	*new;
	int		start;
	int		end;
	int		i;

	start = 0;
	end = ft_strlen(s1);
	i = 0;
	while (s1[start] != '\0' && (checkset(s1[start], set) == 1))
		start++;
	while (end > start && (checkset(s1[end - 1], set) == 1))
		end--;
	new = (char *)malloc(sizeof(char) * ((end - start) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (start < end)
		new[i++] = s1[start++];
	new[i] = '\0';
	if (flag)
		free(s1);
	return (new);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 22:08:40 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:17 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len >= ft_strlen(s))
		len = ft_strlen(s) - start;
	if (len + start >= ft_strlen(s))
		len = ft_strlen(s) - start;
	i = 0;
	sub = (char *)malloc(sizeof(char) * len + 1);
	if (!sub)
		return (NULL);
	while (s[start] != '\0' && i < len)
		sub[i++] = s[start++];
	sub[i] = '\0';
	return (sub);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:59:12 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:25 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:55:22 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:30 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:22:31 by tabadawi          #+#    #+#             */
/*   Updated: 2024/08/21 12:48:35 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_get_line(char *save)
{
	int		i;
	char	*print;

	i = 0;
	if (!save[i])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	print = malloc((sizeof(char)) * (i + 1 + (save[i] == '\n')));
	if (!print)
		return (NULL);
	i = 0;
	while (save[i] != '\n' && save[i] != '\0')
	{
		print[i] = save[i];
		i++;
	}
	if (save[i] == '\n')
	{
		print[i] = '\n';
		i++;
	}
	print[i] = '\0';
	return (print);
}

static char	*ft_save(char *save)
{
	int		i;
	int		j;
	char	*buff;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	buff = malloc ((sizeof(char)) * ft_strlen(save) - i + 1);
	if (!buff)
		return (NULL);
	i++;
	j = 0;
	while (save[i])
		buff[j++] = save[i++];
	buff[j] = '\0';
	free(save);
	return (buff);
}

static char	*gnl_strjoin(char *sv, char *buff)
{
	char	*newsave;
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (!sv && !buff)
		return (NULL);
	newsave = malloc(sizeof(char) * ((ft_strlen(sv) + ft_strlen(buff)) + 1));
	if (!newsave)
		return (NULL);
	while (sv && sv[i] != '\0')
	{
		newsave[i] = sv[i];
		i++;
	}
	while (buff[j] != '\0')
		newsave[i++] = buff[j++];
	newsave[i++] = '\0';
	if (sv)
		free(sv);
	return (newsave);
}

char	*ft_readfile(char *save, int fd)
{
	int		rd;
	char	*buffer;

	buffer = malloc((sizeof(char)) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	rd = 1;
	while (rd != 0 && ft_strchr(save, '\n') == 0)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd == -1)
		{
			free(save);
			free(buffer);
			return (NULL);
		}
		buffer[rd] = '\0';
		save = gnl_strjoin(save, buffer);
	}
	free(buffer);
	return (save);
}

char	*get_next_line(int fd)
{
	char		*print;
	static char	*save[1024];

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	save[fd] = ft_readfile(save[fd], fd);
	if (!save[fd])
		return (NULL);
	print = ft_get_line(save[fd]);
	save[fd] = ft_save(save[fd]);
	return (print);
}


