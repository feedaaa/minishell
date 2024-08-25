/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:29:50 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/25 21:26:30 by ffidha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h>
# include <dirent.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include "libft/libft.h"

# define SUCCESS 0
# define FAILURE -1
# define OPERATORS "|<>"
# define QUOTES "\"\'"
# define DELIMS "\"\' "
# define REDIRECTS "><"

/* ERROR MESSAGES */

// MINISHELL
# define NOT_FOUND 0
# define CL_ARGUMENTS_ERR "minishell: no support for command-line arguments"
# define NO_PIPE_PROMPT "minishell: no support for pipe prompt"
# define NO_SYTX_PROMPT "minishell: no support for syntax prompt"
# define PIPE_ERR "minishell: pipe() failed"
# define FORK_ERR "minishell: fork() failed"

// SYNTAX

# define UNCLOSED_QUOTES "minishell: unclosed quotes"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token `"
# define SYTX_ERR_RDR "minishell: syntax error near unexpected token `newline'"

// EXIT CMD

# define EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments"
# define EXIT_NON_NUMERIC_ARG "minishell: exit: numeric argument required" 

// CD CMD

// # define PATH_MAX	4096
# define CD_TOO_MANY_ARGS "minishell: cd: too many arguments"
# define OLDPWD_NOT_SET "minishell: cd: OLDPWD not set"

extern long long		g_exit_status;

typedef enum e_operator {
	NONE,
	RDR_OUT_REPLACE,
	RDR_OUT_APPEND,
	RDR_INPUT,
	RDR_INPUT_UNTIL,
	PIPE,
}			t_operator;

typedef struct s_vlst {
	char			*var_name;
	char			*var_value;
	bool			is_exported;
	struct s_vlst	*next;
}				t_vlst;

typedef struct s_statement {
	int					argc;
	char				**argv;
	t_operator			operator;
	struct s_statement	*next;
}				t_statement;

typedef struct s_data {
	char		**envp;
	t_vlst		*envp_lst;
	t_statement	*head;
}				t_data;

void			init_oldpwd(t_vlst **head);
int				unset_var(char *var_name, t_vlst **head);
void			invalid_identifer(char *var_name);
int				save_user_vars(char *statement, t_vlst **head, bool to_export);
bool			get_exported_state(char *var_name, t_vlst **head);
int				unset_var(char *var_name, t_vlst **head);
void			v_lstadd_back(t_vlst **head, t_vlst *new);
t_vlst			*v_lstlast(t_vlst *node);
void			dismiss_signal(int signum);
void			config_signals(void);

//Parsing Functions
size_t			get_token_length(char *input_at_i);
char			**tokenize_input(char *input);
bool			is_therechar(const char *str, int ch);
size_t			count_tokens(char *input);
int				ft_strcmp(const char *s1, const char *s2);
t_operator		get_operator(char *operator);
t_statement		*parser(char *input);
size_t			remove_quotes_size(char	*parsed);
char			*remove_quotes(char	*parsed);
size_t			get_argc(char **parsed);
char			*ft_lltoa(long long n);
bool			unexpected_token(char token);
bool			has_operator(char *input);
bool			invalid_syntax_on_operator(char *input);
bool			invalid_syntax2(char *input);
bool			invalid_syntax(char *input);
void			destroy(t_data *data);
bool			valid_input(char *input, t_data *data);
int				unclosed_quotes(char *str);
void			free_matrix(char **matrix);
void			p_lstclear(t_statement **head);
void			v_lstclear(t_vlst **head);

//expanding function
size_t			expand_exit_status(char *expanded_input_at_i, size_t *i);
size_t			expand_variable(char *expanded_input_at_i, char *input,
					size_t *i, t_data *data);
size_t			expand_size(char *input_at_i, size_t *i, t_data *data);
int				expanded_size(char *input, t_data *data);
char			*expander(char *input, t_data *data);
char			*get_fromvlst(char *var_name, t_vlst **head);
char			*get_varvalue_fromvlst(char *var_name, t_data *data);
size_t			exit_status_size(void);
void			init_vars(size_t *i, size_t *size, bool *in_quotes,
					bool *in_dquotes);
bool			single_dollar(char *input_at_i);
bool			streq(char *str1, char *str2);

//setup functions
void			setup_shell(char **envp, t_data *data,
					t_statement **statement_list);
t_vlst			*init_envp_lst(char **envp);
char			**split_envp(char *env);
t_vlst			*v_new_node(char *var_name, char *var_value, bool is_exported);
t_statement		*p_new_node(int argc);

void			cmd_exit(t_statement *s, t_data *data);
bool			fits_in_longlong(char *str);
bool			is_all_digits_or_signals(char *str);
long long		ft_atoll(const char *str);
bool			is_valid_id(char *str);
bool			is_spaces(char c);
char			**get_paths(t_vlst *envp_lst);
void			cmd_binaries(t_statement *statement, t_data *data);
void			exit_and_free_matrix(char **paths, char *cmd, int exit_status);
char			*get_bin_path(char *cmd, char **paths);
char			*join_free(char *s1, char *s2);
bool			is_absolute_path(t_statement *statement);
void			cmd_not_found(char *cmd_name);
void			exec_pipe(t_statement *node, t_data *data);
void			exec_cmd(t_statement *current_node, t_data *data);
void			child_signals(int signum);
void			exec_executables(t_statement *node, t_data *data);
bool			builtin(t_statement *s, t_data *data);
size_t			p_lstsize(t_statement *head);
void			exec_type(t_statement *statement_list, t_data *data);
void			panic(t_data *data, char *error_msg, int exit_status);
int				cmd_env(t_data *data);
void			exec_redirects(t_statement *node, t_data *data);
int				cmd_export(t_statement *statement, t_data *data);
int				cmd_unset(t_statement *s, t_vlst **head);
int				cmd_cd(char *path, t_data *data);
int				call_cmd_unset(t_statement *s, t_data *data);
int				call_cmd_cd(t_statement *s, t_data *data);
int				call_cmd_echo(t_statement *s);
int				cmd_pwd(void);
int				cmd_echo(t_statement *statement, bool has_n);
void			child_heredoc(int write_fd, char *delimiter);

//clean-free function
void			clean_parsed(t_statement **statement_list, t_data *data);
void			clean_expander(t_statement *command);
int				export_bad_identifier(char *identifier);
void			rl_replace_line(const char *s, int c);
void            rl_clear_history(void);
void			exit_non_numeric_arg(void);

#endif