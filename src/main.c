/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2025/12/19 13:59:39 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char		*input;
	t_history	*history_list;

	history_list = NULL;


	input = readline("Minishell > ");
	while (input != NULL)
	{
		printf("You entered: %s\n", input);
		if (input[0] != '\0')
		{
			add_history(input);
			add_to_history(input, &history_list);
		}
		if (ft_strncmp(input, "history", 8) == 0)
			display_history(history_list);
		else if (ft_strncmp(input, "exit", 5) == 0)
			break ;
		free(input);
		input = readline("Minishell > ");
	}
	free_history(&history_list);
	return (0);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands. Without this, your shell would lose its interactive 
command recall feature.
*/