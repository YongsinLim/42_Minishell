/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/01/13 15:06:18 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     main(int argc, char **argv, char **envp)

{
	char		*input;
	t_history	*history_list;
	t_token		*tokens;
	t_command	*pipeline;
	int			status;

	(void)argc;
	(void)argv;

	history_list = NULL;
	status = 0;
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
		tokens = tokenize(input, envp);




		pipeline = parse(tokens);
		if (pipeline != NULL)
		{
			handle_heredocs(pipeline);
			status = execute_pipeline(pipeline, envp);
		}
		free(input);
		free_tokens(&tokens);
		free_pipeline(&pipeline);
	}
	free_history(&history_list);
	
	return (status);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands. Without this, your shell would lose its interactive 
command recall feature.
*/