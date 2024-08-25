
#include "minishell.h"

void	dismiss_signal(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 1;
	}
}

// function to control the signals behavior
void	config_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &dismiss_signal; // By setting it to &dismiss_signal,when the signal is SIGINT= ctrl C dismiss _signal function is applied
	sa.sa_flags = SA_RESTART;       
		// This prevents system calls from failing unexpectedly due to signal interruptions
	sigemptyset(&sa.sa_mask);        // initializes a signal set to be empty
	sigaddset(&sa.sa_mask, SIGINT); 
		// adds the specified signal (SIGINT) to the signal set
	sigaction(SIGINT, &sa, NULL);   
		// Applies the configured signal action for SIGINT
	signal(SIGQUIT, SIG_IGN);        // Sets the action for SIGQUIT = 'ctrl/' to be ignored
}

// ctrl+D is not considered as a signal cause when you input something in the command line and click on ctrl+D nothing would happen so you will have to add it in the builtins
