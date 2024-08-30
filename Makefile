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
BUILT		=	builtins
PARSE		=	parser
EXEC		=	execution

SRCS	=		$(EXEC)/binaries.c \
				$(BUILT)/builtins_cmd.c \
				$(BUILT)/cd.c \
				$(EXEC)/config_signals.c \
				$(BUILT)/echo.c \
				$(BUILT)/env.c \
				$(PARSE)/error_msg.c \
				$(EXEC)/exec_type.c \
				$(BUILT)/exit.c \
				$(PARSE)/exp_utils.c \
				$(PARSE)/exp_utils2.c \
				$(PARSE)/expand.c \
				$(BUILT)/export.c \
				$(PARSE)/free_clean.c \
				$(EXEC)/heredoc.c \
				$(PARSE)/input_utils.c \
				$(PARSE)/invalid_syntax.c\
				$(PARSE)/invalid_syntax2.c\
				$(EXEC)/old_pwd.c \
				$(PARSE)/parser_utils.c \
				$(PARSE)/parser.c \
				$(EXEC)/pipe.c \
				$(BUILT)/pwd.c \
				$(EXEC)/redirects.c \
				$(PARSE)/setup_shell.c \
				$(PARSE)/tokenizer.c \
				$(BUILT)/unset.c \
				$(PARSE)/valid_input.c \
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
