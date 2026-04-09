/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/04/09 18:49:25 by yolim            ###   ########.fr       */
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

add_history(str): allow users to press the Up / Down arrow to see previous commands

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
	char		*raw_line;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
		init_signals_prompt();
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			raw_line = readline("Minishell > ");
		else
			raw_line = get_next_line(STDIN_FILENO);
		if (raw_line)
			minishell.input = ft_strtrim(raw_line, "\r\n");
		free(raw_line);
		minishell.input = check_unclosed_quotes(minishell.input, interactive);
		if (!minishell.input)
		{
			if (interactive)
				ft_putstr_fd("exit\n", 2); // on EOF or Ctrl + D
			break ;
		}
		if (!is_all_whitespace(minishell.input))
		{
			if (interactive)
				add_history(minishell.input);
			add_to_history(minishell.input, &minishell.history_list);
		}
		minishell.tokens = tokenize(minishell.input, &minishell);
		// ---------------------------------------------------------------------














		if (!minishell.tokens)
		{
			free(minishell.input);
			minishell.input = NULL;
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

// ---------------------------------------------------------------------







void	execution(t_minishell *minishell)
{
	minishell->ast = parse(&minishell->tokens);
	if (minishell->ast != NULL)
	{
		heredocs(minishell->ast, minishell);
		minishell->last_exit_status = execute_ast(minishell->ast, minishell);
	}
	else
		minishell->last_exit_status = SYNTAX_ERROR;
	free(minishell->input);
	minishell->input = NULL;
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
}

void	cleanup_and_exit(t_minishell *minishell, int exit_status)
{
	free_env_list(minishell->env_list);
	free_history(&minishell->history_list);
	free(minishell->input);
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
	rl_clear_history(); // TODO : check what function do
	exit(exit_status);
}
