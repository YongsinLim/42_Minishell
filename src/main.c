/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/02/22 18:17:38 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int g_exit_status = 0; // The only declaration

static int	is_empty_line(char *line)
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
}

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
		if (input == NULL) // Ctrl+D (EOF)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (is_empty_line(input))
		{
			free(input);
			continue ;
		}
		add_history(input);
		add_to_history(input, &history_list);
		if (has_unclosed_quotes(input))
		{
			ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
			free(input);
			continue ;
		}
		if (ft_strncmp(input, "history", 8) == 0)
			display_history(history_list);
		else if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break ;
		}
		tokens = tokenize(input, env_list); 
		ast = parse(&tokens);
		if (ast != NULL)
		{
			heredocs(ast, &env_list); 
			status = execute_ast(ast, &env_list);
		}
		
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