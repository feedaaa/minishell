# valgrind --quiet --trace-children=yes --track-fds=yes ./minishell
valgrind  --suppressions=readline.supp --leak-check=full --trace-children=yes --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell

# valgrind --suppressions=readline.supp --leak-check=full --trace-children=yes --show-leak-kinds=all --show-leak-kinds=all ./minishell
# valgrind --quiet --track-fds=yes ./minishell
# valgrind --tool=memcheck --tool=callgrind --num-callers=20 --track-fds=yes --error-exitcode=1 ./minishell
# valgrind -q --trace-children=yes  --track-fds=yes ./minishell
# valgrind --track-fds=yes ./minishell
# valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes -v ./minishell
# valgrind -s --track-fds=yes --tool=memcheck --leak-check=full --show-leak-kinds=all --suppressions=.ignore_readline ./minishell
# --suppressions=readline.supp