/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/03/01 19:58:28 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int g_exit_status = 0; // The only declaration

/*static int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (1);
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

static int	has_unclosed_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}*/

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argc;
	(void)argv;
	minishell.history_list = NULL;
	minishell.env_list = init_env(envp);
	minishell.last_exit_status = 0;
	minishell.input = NULL;
	minishell.tokens = NULL;

	// init_signals_prompt();
	while (1)
	{
		minishell.input = readline("Minishell > "); // handle in terminal or not in terminal
		if (!minishell.input)
		{
			ft_putstr_fd("exit\n", 2); // on EOF or Ctrl + D
			break ;
		}
		minishell.tokens = tokenize(minishell.input, &minishell);
		if (!minishell.tokens)
		{
			ft_putstr_fd("minishell : Unclosed quote found\n", 2);
			free(minishell.input);
			continue ;
		}
		execution(&minishell);
	}
	free_env_list(minishell.env_list);
	free_history(&minishell.history_list);
	rl_clear_history();    // TODO : check what is this
	return (minishell.last_exit_status);
}

void	execution(t_minishell *minishell)
{
	if (minishell->input[0] != '\0')
	{
		add_history(minishell->input);
		add_to_history(minishell->input, &minishell->history_list);
	}
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
	rl_clear_history();
	exit(exit_status);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands.
*/