/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:29:50 by ffidha            #+#    #+#             */
/*   Updated: 2024/08/14 13:19:00 by ffidha           ###   ########.fr       */
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
#include "libft/libft.h"

# define OPERATORS "|<>"
# define QUOTES "\"\'"

extern int		g_exit_status;


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

//shell struct
typedef	struct s_shell {
	
	int			fd;
	pid_t		child;
	t_vlst		**env;
	//executer
	//environment
}t_shell;

// env struct
typedef struct s_envrn {
	t_shell		*shell;
		
}t_envrn;

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

//expanding function
size_t	expand_exit_status(char *expanded_input_at_i, size_t *i);
size_t	expand_variable(char *expanded_input_at_i, char *input,
	size_t *i, t_data *data);
size_t	expand_size(char *input_at_i, size_t *i, t_data *data);
int	expanded_size(char *input, t_data *data);
char	*expander(char *input, t_data *data);
char	*get_fromvlst(char *var_name, t_vlst **head);
char	*get_varvalue_fromvlst(char *var_name, t_data *data);
size_t	exit_status_size(void);
void	init_vars(size_t *i, size_t *size, bool *in_quotes, bool *in_dquotes);
bool	single_dollar(char *input_at_i);

//setup functions
void	setup_shell(char **envp, t_data *data, t_statement **statement_list);
t_vlst	*init_envp_lst(char **envp);
char	**split_envp(char *env);
t_vlst	*v_new_node(char *var_name, char *var_value, bool is_exported);
t_statement	*p_new_node(int argc);

#endif
