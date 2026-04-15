/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/04/15 20:46:05 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
int isatty(int fd) :
isatty is a function used to determine if a file descriptor is associated
with a terminal device (such as a console or serial port) rather than a
regular file or pipe

Returns: 1 if the descriptor refers to a terminal;
		 0 otherwise

STDIN_FILENO is a macro that represents the integer file descriptor for the
standard input stream.

readline:  readi a line (user input) from the terminal with interactive
features like history and tab completion

add_history(str): allow users to press the Up / Down arrow to see previous
commands

Use readline for interactive :
Requires a terminal (TTY) to work properly
Shows a prompt string
Provides interactive features: history, editing, completion
Returns string WITHOUT the newline character
Allocates memory that needs to be freed
Will fail or behave unexpectedly with pipes/files

Use GNL for non-interactive :
Works with any file descriptor (stdin, pipes, files)
No prompt support - just reads
No interactive features
Returns string WITH the newline character

!minishell.input :
User presses Ctrl+D (EOF signal) in interactive mode
readline() returns NULL
minishell.input becomes NULL
Print "exit\n" to stderr
Break out of the loop and exit the shell
*/

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	int			interactive;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
		init_signals_prompt();
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (!read_and_prepare_input(&minishell, interactive))
			break ;
		if (!build_tokens(&minishell))
		{
			if (!interactive && minishell.last_exit_status == SYNTAX_ERROR)
				break ;
			continue ;
		}
			execution(&minishell);
		if (!interactive && minishell.last_exit_status == SYNTAX_ERROR)
			break ;
	}
	cleanup_and_exit(&minishell, minishell.last_exit_status);
}

//ft_putstr_fd("exit\n", 2) on EOF or Ctrl + D in interactive mode
int	read_and_prepare_input(t_minishell *minishell, int interactive)
{
	char	*raw_line;

	if (interactive)
		raw_line = readline("Minishell > ");
	else
		raw_line = get_next_line(STDIN_FILENO);
	// ---------------------------------------------------------------------

		if (g_signal == SIGINT)
		{
			minishell -> last_exit_status = 130;
			g_signal = 0; //Reset signal after receiving;
		}
	// ---------------------------------------------------------------------

	if (!raw_line)
	{
		if (interactive)
			ft_putstr_fd("exit\n", 2);
		return (FALSE);
	}
	minishell->input = ft_strtrim(raw_line, "\r\n");
	free(raw_line);
	minishell->input = check_unclosed_quotes(minishell->input, interactive);
	if (!minishell->input)
		return (FALSE);
	if (!is_all_whitespace(minishell->input))
	{
		if (interactive)
			add_history(minishell->input);
		add_to_history(minishell->input, &minishell->history_list);
	}
	return (TRUE);
}

int	is_all_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	build_tokens(t_minishell *minishell)
{
	minishell->tokens = tokenize(minishell->input, minishell);
	if (minishell->tokens)
		return (TRUE);
	free(minishell->input);
	minishell->input = NULL;
	return (FALSE);
}





/*
rl_clear_history() is used to completely clear the command history list and
free the memory associated with its entries
*/
void	cleanup_and_exit(t_minishell *minishell, int exit_status)
{
	free_env_list(minishell->env_list);
	free(minishell->input);
	free_history(&minishell->history_list);
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
	rl_clear_history();
	exit(exit_status);
}

// ---------------------------------------------------------------------






/*
Execution is called every loop iteration (every command).
Cleanup is needed to clean memory from the current command
before reading the next one.

So, execution() cleanup = end of one command lifecycle
cleanup_and_exit() cleanup = end of whole shell lifecycle
 */
void	execution(t_minishell *minishell)
{
	minishell->ast = parse(&minishell->tokens);

	if (minishell->ast != NULL)
	{
		// Only execute the AST if heredocs were NOT interrupted
		if (heredocs(minishell->ast, minishell) != 130)
			minishell->last_exit_status = execute_ast(minishell->ast, minishell);
	}
	else
		minishell->last_exit_status = SYNTAX_ERROR;
		
	free(minishell->input);
	minishell->input = NULL;
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
}
