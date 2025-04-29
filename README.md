
# Minishell

This project is a simplified version of the Unix shell (like bash), implemented in C. It's a command-line interpreter that allows users to execute basic commands, manage processes, and interact with the file system.  This was a collaborative project.


## Features

he minishell supports the following features:

Command Execution: Execute standard Unix commands (e.g., ls, pwd, cd, echo, cat).

Built-in Commands: Implementation of built-in commands:

`cd`: Change the current directory.

`pwd`: Print the current working directory.

`echo`: Display a line of text.

`exit`: Terminate the shell.

`env`: Print the environment variables.

`unsetenv`: Unset an environment variable.

`setenv`: Set an environment variable.

Path Handling: Search for executable commands in the directories listed in the PATH environment variable.

Redirection:

Input redirection (<): Redirect standard input from a file.

Output redirection (>): Redirect standard output to a file (overwrites).

Append output redirection (>>): Redirect standard output to a file (appends).

Piping: Connect the output of one command to the input of another using the pipe operator (|).

Signal Handling: Handle interrupts (Ctrl+C) and quits (Ctrl+\) to prevent the shell from crashing.

Error Handling: Display informative error messages for invalid commands or operations.

Prompt: Display a user-friendly prompt.

Variable Substitution: Expand environment variables (e.g., `$HOME`, `$USER`).


## Usage/Examples

`$ ./minishell
my_shell$ ls -l
total 12
-rwxr-xr-x 1 user user 8648 Oct 26 10:00 minishell
-rw-r--r-- 1 user user  245 Oct 26 09:58 main.c
my_shell$ pwd
/home/user/minishell
my_shell$ cd /tmp
my_shell$ pwd
/tmp
my_shell$ echo "Hello, world!"
Hello, world!
my_shell$ cat < input.txt > output.txt
my_shell$ ls -l | grep .c
-rw-r--r-- 1 user user  245 Oct 26 09:58 main.c
my_shell$ exit`


## Contributers

- Fathima Fidha (me)

- Aysha Bataineh
