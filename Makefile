NAME	=	minishell

CC		=	cc

FLAGS	=	-Wall -Werror -Wextra

RLFLAGS	=	-lreadline -lhistory
RLDIR	=	-L/opt/vagrant/embedded/lib
RLINC	=	-I/opt/vagrant/embedded/include/readline/readline.h

LIBFT	=	libft/libft.a

SRC	=			parser_utils.c \
				tokenizer.c \
				parser.c \
				setup_shell.c \
				expand.c \
				exp_utils.c \
				valid_input.c\
				input_utils.c \
				invalid_syntax.c\
				minishell.c 

OBJ	=	$(SRC:.c=.o)

all	:	$(NAME)

$(LIBFT):
	$(MAKE)	-C libft
	
$(NAME)	:	$(LIBFT) $(OBJ)
	$(CC) $(FLAGS) $(RLDIR) $(RLINC) $(OBJ) $(LIBFT)  -o $@ $(RLFLAGS)
	clear
	echo 'Compiled!'

%.o: %.c
	$(CC) -I $(INC) $(FLAGS) -c $< -o $@

RM	:	rm -rf

clean		:
	$(RM)

fclean		:
	clean $(NAME)

re	: 	fclean all

.PHONY	:	all	clean	fclean	re
