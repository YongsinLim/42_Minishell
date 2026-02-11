/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/02/11 18:02:45 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)

{
	t_env		*env_list;
	char		*input;
	t_history	*history_list;
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	(void)argc;
	(void)argv;

	history_list = NULL;
	status = 0;
	env_list = init_env(envp);
	while (1)
	{
		input = readline("Minishell > ");
		if (input == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			add_to_history(input, &history_list);
		}
		if (ft_strncmp(input, "history", 8) == 0)
			display_history(history_list);
		else if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break ;
		}

		tokens = tokenize(input, env_list);
		if (!tokens)
			error_exit("minishell : Unclosed quote found\n");
		ast = parse(&tokens);
		if (ast != NULL)
		{
			heredocs(ast, env_list);
			status = execute_ast(ast, &env_list);
		}
		print_ast(ast, 0);
		free(input);
		free_tokens(&tokens);
		free_ast(&ast);
	}
	free_env_list(env_list);
	free_history(&history_list);
	return (status);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands.
*/