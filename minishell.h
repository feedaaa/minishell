/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:29:50 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/22 14:00:03 by ffidha           ###   ########.fr       */
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

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

typedef struct s_statement {
    int                 argc; //The number of commands
    char              **argv; //The commnands between each operstor
    t_operator         operator;
	struct s_statement	*next;
} t_statement;


//Lets assume NAME=VALUE so Name is saved in var_name and Value is saved in var_value
typedef struct s_vlst {
	char			*var_name;
	char			*var_value;
	bool			is_exported;
	struct s_vlst	*next;
}				t_vlst;

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
	t_operator	operator;
	int			exit_status;
	int			pipe_fds[2];
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


// //execution functions
// void    	execution(t_statement *parsed_commands, char **env);
// void 		execute_command(char *command, char **args, char **env);
// int			is_builtin(char *command);
// void		execute_builtin(t_statement *command);
// void 		exec_one_cmd(t_statement *parsed_command, char **env);
// void		execute_pipe(t_statement *parsed_commands, char **env);


// //execution utils
void execute_command(t_statement *command);
// char    	*ft_strcpy(char *dst, const char *src);
// t_vlst		*msh_lstnew(char *var_name, char *var_value);
// void		msh_lstadd_back(t_vlst **lst, t_vlst *new);
// void		msh_lstdelone(t_vlst *lst, void (*del)(void*));

//clean-free function
void		clean_parsed(t_statement **statement_list, t_data *data);


//builtin functions
int			msh_pwd(t_statement *command);
void		msh_echo(t_statement *command);
int			msh_env(t_vlst *head);
void		msh_cd(t_statement *command);
int			msh_export(char *var_name, char *var_value, t_vlst **head);
int 		msh_unset(char *var_name, t_vlst **head);

// //heredoc functions
// int 		hd_child(t_statement *node, int fd);
// int 		here_doc(t_statement *node);

#endif
