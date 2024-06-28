NAME		= Minishell
CC			= cc
FLAGS		= -Wall -Werror -Wextra
SRC			= main.c prompt.c 

OBJ := $(SRC:%.c=%.o)


%.o: %.c
	$(CC) $(CFLAGS) $(OBJ)
	
all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS)


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all