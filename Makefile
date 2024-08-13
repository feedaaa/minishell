NAME: minishell

CC		: cc

FLAGS	: -Wall -Werror -Wextra -lreadline -g3

LIBFT_PATH := libft
LIBFT := $(LIBFT_PATH)/libft.a
LIBFT_LINK := -I$(LIBFT_PATH) -L$(LIBFT_PATH) -lft

SRC		:	minishell.c \
			tokenizer.c \
			parser_utils.c \
			parser2.c \
			setup_shell.c \
			expand.c		\
			exp_utils.c		\


OBJ		: $($(SRC:.c=.o))

$(LIBFT):
	make -C $(LIBFT_PATH)

all		: $(NAME)

$(NAME)	:	$(LIBFT) $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT_LINK) -o $(NAME)
	clear
	@echo 'Compiled!'


RM		: rm -rf

clean	:
	@$(RM) $(OBJ) $(LIBFT_PATH)

fclean	:
	clean $(NAME)

re: fclean all

.PHONY	: all clean fclean re