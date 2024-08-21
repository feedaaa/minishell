NAME		=	minishell
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -g3
# CFLAGS		+=	-fsanitize=address

# the following flagsg are to be used on 42 computers
RLFLAGS		=	-lreadline -lhistory
RLDIR		=	-L/opt/vagrant/embedded/lib
RLINC		=	-I/opt/vagrant/embedded/include/readline/readline.h

# and the following flags are to be used on personal computers
# RLDIR += -L/usr/local/opt/readline/lib
# RLINC += -I/usr/local/opt/readline/include
# RLFLAGS = -lreadline -lhistory

LIBFT		=	libft/libft.a
# INC			=	shell/includes
# SRCSPATH	=	shell/srcs
BUILT		=	builtins
# EXEC		=	$(SRCSPATH)/execution
# LEX			=	$(SRCSPATH)/lex
# SIG			=	$(SRCSPATH)/signals
# UTILS		=	$(SRCSPATH)/utils
# ENV			=	$(SRCSPATH)/environ

SRCS	=		parser_utils.c \
				tokenizer.c \
				parser.c \
				setup_shell.c \
				expand.c \
				exp_utils.c \
				valid_input.c\
				input_utils.c \
				invalid_syntax.c\
				old_pwd.c \
				execution.c \
				exec_utils.c \
				pipe.c \
				heredoc.c \
				clean_parsed.c\
				config_signals.c \
				$(BUILT)/msh_pwd.c \
				$(BUILT)/msh_echo.c \
				$(BUILT)/msh_env.c \
				$(BUILT)/msh_cd.c \
				$(BUILT)/builtin.c \
				minishell.c \


OBJS		=	$(SRCS:.c=.o)

all		:	$(NAME)


$(LIBFT):
	@$(MAKE)	-C libft
	
$(NAME)	:	$(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(RLDIR) $(RLINC) $(OBJS) $(LIBFT)  -o $@ $(RLFLAGS)
	clear
	@echo "minishell compiled !"

%.o: %.c
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@

clean	:
	@$(MAKE)	-C libft clean
	@rm -rf		$(OBJS)
	clear
	@echo "minishell cleaned !"

fclean	:	clean
	@$(MAKE)	-C libft fclean
	@rm -rf		$(NAME)
	clear
	@echo "minishell cleaned !"

re		:	fclean	all

.PHONY:	all clean fclean re

# $(BUILT)/msh_echo.c \
# $(BUILT)/msh_env.c \
# $(BUILT)/msh_exit.c \
# $(BUILT)/msh_export.c \