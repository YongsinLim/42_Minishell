/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:31:16 by yolim             #+#    #+#             */
/*   Updated: 2025/12/30 18:29:06 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next_node;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next_node = current->next;
		free(current->value);
		free(current);
		current = next_node;
	}
	*tokens = NULL;
}

void	free_pipeline(t_command **pipeline)
{
	t_command	*current;
	t_command	*next_node;

	if (!pipeline || !*pipeline)
		return ;
	current = *pipeline;
	while (current)
	{
		next_node = current->next;
		free_array_str(current->argv);
		free(current->input_file);
		free(current->output_file);
		free(current->heredoc_delimiter);
		if (current->heredoc_fd != -1)
			close(current->heredoc_fd);
		free(current);
		current = next_node;
	}
	*pipeline = NULL;
}

void	free_array_str(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
