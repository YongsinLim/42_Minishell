/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/02/12 13:34:35 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	char		*trimmed_input;
	t_minishell	minishell;
	t_token		*tokens;

	(void)argc;
	(void)argv;
	minishell.history_list = NULL;
	minishell.envp = envp;
	minishell.last_exit_status = 0;
	while (1)
	{
		input = readline("Minishell > ");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			free(input); // on EOF or Ctrl + D
			break ;
		}
		trimmed_input = ft_strtrim(input, " \t\n\r\f\v");
		if (!trimmed_input) // eg. space key in for input
		{
			free(input);
			error_exit("minishell : Failed to trim input_line\n");
		}
		if (trimmed_input[0] == '\0')
		{
			free(input);
			free(trimmed_input);
			continue ;
		}
		if (ft_strncmp(trimmed_input, "history", 7) == 0
			&& ft_strlen(trimmed_input) == 7)
		{
			display_history(minishell.history_list);
			if (input[0] != '\0')
			{
				add_history(input);
				add_to_history(input, &minishell.history_list);
			}
			free(input);
			free(trimmed_input);
			continue ;
		}
		else if (ft_strncmp(trimmed_input, "exit", 4) == 0
			&& ft_strlen(trimmed_input) == 4)
		{
			ft_putstr_fd("exit\n", 1);
			free(input);
			free(trimmed_input);
			break ;
		}
		tokens = tokenize(input, &minishell);
		if (!tokens)
		{
			ft_putstr_fd("minishell : Unclosed quote found\n", 2);
			free(input);
			free(trimmed_input);
			continue ;
		}
		execution(input, tokens, &minishell);
		free(trimmed_input);
	}
	free_history(&minishell.history_list);
	return (minishell.last_exit_status);
}

void	execution(char *input, t_token *tokens, t_minishell *minishell)
{
	t_ast_node	*ast;

	if (input[0] != '\0')
	{
		add_history(input);
		add_to_history(input, &minishell->history_list);
	}
	ast = parse(&tokens);
	if (ast != NULL)
	{
		heredocs(ast, minishell);
		minishell->last_exit_status = execute_ast(ast, minishell);
	}
	else
	{
		ft_putstr_fd("minishell : syntax error\n", 2);
		minishell->last_exit_status = SHELL_FAILURE;
	}
	// print_ast(ast, 0);
	free(input);
	free_tokens(&tokens);
	free_ast(&ast);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands.
*/