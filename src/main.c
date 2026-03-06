/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/03/06 16:30:08 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	char		*line;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);

	// init_signals_prompt();
	while (1)
	{


		// to check if stdin is a terminal using isatty(STDIN_FILENO):
		if (isatty(STDIN_FILENO))
			minishell.input = readline("Minishell > "); // handle in terminal or not in terminal
		else {
			line = get_next_line(STDIN_FILENO);
			if (line) {
				minishell.input = ft_strtrim(line, "\n");
				free(line);
			}
			else
				minishell.input = NULL;
		}




		if (!minishell.input)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", 2); // on EOF or Ctrl + D
			break ;
		}
		if (!is_all_whitespace(minishell.input))
		{
			add_history(minishell.input);
			add_to_history(minishell.input, &minishell.history_list);
		}
		minishell.tokens = tokenize(minishell.input, &minishell);
		if (!minishell.tokens)
		{
			free(minishell.input);
			continue ;
		}
		execution(&minishell);
	}
	cleanup_and_exit(&minishell, minishell.last_exit_status);
}

void	init_minishell(t_minishell *minishell, char **envp)
{
	minishell->history_list = NULL;
	minishell->env_list = init_env(envp);
	minishell->last_exit_status = 0;
	minishell->input = NULL;
	minishell->tokens = NULL;
	minishell->ast = NULL;
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

void	execution(t_minishell *minishell)
{
	minishell->ast = parse(&minishell->tokens);
	if (minishell->ast != NULL)
	{
		heredocs(minishell->ast, minishell);
		minishell->last_exit_status = execute_ast(minishell->ast, minishell);
	}
	else
	{
		ft_putstr_fd("minishell : syntax error\n", 2);
		minishell->last_exit_status = SHELL_FAILURE;
	}
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

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands.
*/