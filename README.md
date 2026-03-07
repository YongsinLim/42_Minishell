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
2. Fix Header File's Norm Issue

RoadMap - 19.12.2025:
1. Display a prompt
2. Create working history (History folder)

RoadMap - 02.01.2026:
1. Implement Pipes & Redirection (Execute folder and free_memory folder)
2. Revise Makefile & Header file
3. Fix Norm
4. main.c : still havent fix function too long issue

RoadMap - 18.1.26
1. Changed pipeline to ats
2. "Builtin" commands working, but no native to program, using the local shell's commands instead

RoadMap - 19.01.2026:
1. implement ’ (single quote) and " (double quote)
2. implement environment variables
** Not done: avoid case of \ (backslash) or ; (semicolon) ;
			$? which should expand to the exit status of the most recently executed
			foreground pipeline.

RoadMap - 30.01.2026:
1. add tokenize "(" ")" "&&" "||" logic

RoadMap - 06.02.2026
1. apply ast to commands
2. add macros for exit status

RoadMap - 08.02.2026
1. implement execution for ast command
2. fix system hang issue

RoadMap - 09.02.2026
1. include heredoc handling
2. fix norm

RoadMap - 11.2.2026
1. Changed char** envp to t_env env 
2. added builtins of echo, pwd and env 
3. Changed logic of expand to fit a linked list

RoadMap - 12.02.2026
1. fix history cmd display error msg - minishell command not found
2. implement $?
3. fix real exit code return

RoadMap - 28.02.2026
1. Builtin (exit) - all cases testing on exit, exit 44, exit abc and exit 12 34 done
2. Merge history, env, pwd function into Builtin function
3. Builtin (Echo) - implement no -n, -n, -nnnn, -n -n -n -n cases

RoadMap - 02.03.2026
1. Add Builtin (Unset)
2. Builtin (env) - fix issue of moving system env list to NULL in 1st called (env). Use local variable to store the head address of the env list to avoid this issue.
3. Builtin (cd) - implement no argument, '-', '..', path
4. Tilde ～ - implement '~' in handle_word
5. Combine get_var_value and get_env_value to avoid duplicate function

RoadMap - 04.03.2026
1. Builtin (export) - add export function, combine is_valid_identifier & check_valid_identifier
2. Move env folder into builtins folder

RoadMap - 05.03.2026
1. fix $"..." $'...', $"$USER" and $'$USER' print '$' issue 
2. handle add_history issue - input (all space / enter)

RoadMap - 07.03.2026
1. fix norm - function too long & line too long issue

	Issue Lists (discovered and unfixed)
```
	   	1. Ctrl + C while in cat heredoc mode ends the entire shell, not just the cat. THeoretically this should be handled with enviroment variable shell level, or signal issue
	   	2. runnign echo with only one " at the start still deletes the ", while it should enter heredoc and wait for next " to display. Might be tokenising or heredoc in parser
	   	3. Same issue for single " after string. It simply deletes the ", while it should also enter heredoc.
```

Pending to do:
1. Makefile : change wildcard, add individual file (mandatory)
2. Tokenize : handle unclosed " or ' issue
3. ReadMe for Minishell
4. ctrl + C direct exit, should be prompt new line
5. ctrl+C in terminal and child behave differently
6. implement wildcard
8. tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
8. fix error in tester


[Process completed]
