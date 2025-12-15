Welcome to Minishell Project

Reminder:
1. Git Pull all latest changes from main branch before start any new changes
2. Branch out (Create new branch) from main branch
3. Do code at that new branch
4. Git Commit & Push to that new branch
5. Create Pull Request when features are ready
6. Merge to main branch via pull request
Extra : During pull request, if there is conflict, pull latest changes from main branch, then merge main to your branch, resolve conflict locally, then only push to remote and merge via pull request


RoadMap - 31.10.2025:
1. Upload Libft
2. Created README

RoadMap - 01.11.2025:
1. includes folder created, added minishell.h
2. Added signal handling, with specific cases as follows

| Mode           | Function                 | Used When                             | `CTRL-C` (`SIGINT`)                            | `CTRL-\` (`SIGQUIT`)                    | `CTRL-D`                            | Purpose                                                |
| -------------- | ------------------------ | ------------------------------------- | ---------------------------------------------- | --------------------------------------- | ----------------------------------- | ------------------------------------------------------ |
| 🟢 **Prompt**  | `init_signals_prompt()`  | Waiting for user input (`readline()`) | Prints newline, clears line, redisplays prompt | Ignored                                 | Handled manually (EOF → exit shell) | Keeps shell interactive; doesn’t quit on interrupt     |
| 🟡 **Child**   | `init_signals_child()`   | After `fork()`, before `execve()`     | Terminates running command                     | Terminates command and prints `Quit: 3` | —                                   | Restores default signal behavior for external programs |
| 🔵 **Heredoc** | `init_signals_heredoc()` | During heredoc (`<<`) input           | Aborts heredoc cleanly                         | Ignored                                 | —                                   | Cancels heredoc without killing shell                  |


Example usage is as follows


	#include "minishell.h"
	int	main(void)
	{
		char	*input;
	
		// Setup signals for prompt mode
		init_signals_prompt();
	
		while (1)
		{
			input = readline("minishell$ ");
			if (!input) // CTRL-D pressed (EOF)
			{
				printf("exit\n");
				break;
			}
	
			// If user entered something, handle it
			if (*input)
				add_history(input);
	
			// Example: handle heredoc separately
			if (is_heredoc(input))
			{
				init_signals_heredoc();
				handle_heredoc(input);
				init_signals_prompt(); // restore prompt behavior
			}
			else
			{
				pid_t pid = fork();
				if (pid == 0)
				{
					// Child process → restore default signals
					init_signals_child();
					exec_cmd(input); // replace process image
					exit(EXIT_FAILURE);
				}
				else if (pid > 0)
					waitpid(pid, NULL, 0);
			}
	
			free(input);
		}
		return (0);
	}


RoadMap - 15.12.2025:
1. Add Makefile
2. Fix Header File's Norm Isue