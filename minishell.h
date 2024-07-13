/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffidha <ffidha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:29:50 by ffidha            #+#    #+#             */
/*   Updated: 2024/07/06 20:44:00 by ffidha           ###   ########.fr       */
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

# define OPERATORS "|<>"
# define QUOTES "\"\'"

typedef enum e_operator {
	NONE,                   //No specific operator.
	RDR_OUT_REPLACE,        //> in shells
	RDR_OUT_APPEND,        //>> in shells
	RDR_INPUT,             //< in shells
       RDR_INPUT_UNTIL,        //<< in shells
	PIPE,                  //| in shells
}t_operator;

typedef struct s_statement {
    int                 argc;
    char              **argv;
    t_operator         operator;
} t_statement;

typedef struct s_commandinput {
    char *command;          // Command string
    t_operator operator;    // Operator associated with the command
} t_commandinput;

//Parsing Functions
size_t get_token_length(char *input_at_i);
char **tokenize_input(char *input) ;
bool is_spaces(char c);
bool	is_therechar(const char *str, int ch);
size_t	count_tokens(char *input);
int	ft_strcmp(const char *s1, const char *s2);
t_operator	get_operator(char *operator);
t_statement *parser(char *input, int *size);
size_t	remove_quotes_size(char	*parsed);
char	*remove_quotes(char	*parsed);
size_t	get_argc(char **parsed);




#endif
