/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 12:49:55 by yolim             #+#    #+#             */
/*   Updated: 2026/04/14 20:17:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	save_input_to_history(t_minishell *minishell, int interactive)
{
	t_history	*new_node;

	if (!is_all_whitespace(minishell->input))
	{
		if (interactive)
			add_history(minishell->input);
		if (!minishell->input || minishell->input[0] == '\0')
			return ;
		new_node = malloc(sizeof(t_history));
		if (!new_node)
			return ;
		new_node->command = ft_strdup(minishell->input);
		if (!new_node->command)
		{
			free(new_node);
			return ;
		}
		new_node->next = NULL;
		append_node_to_history_list(minishell, new_node);
	}
}

void	append_node_to_history_list(t_minishell *minishell, t_history *new_node)
{
	t_history	*last;

	if (minishell->history_list == NULL)
		minishell->history_list = new_node;
	else
	{
		last = minishell->history_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
}

void	free_history(t_history **history_list)
{
	t_history	*current;
	t_history	*next_node;

	current = *history_list;
	while (current != NULL)
	{
		next_node = current->next;
		free(current->command);
		free(current);
		current = next_node;
	}
	*history_list = NULL;
}

int	ft_history(t_history *history_list)
{
	t_history	*current;
	int			i;

	current = history_list;
	i = 1;
	while (current != NULL)
	{
		printf(" %d  %s\n", i, current->command);
		i++;
		current = current->next;
	}
	return (SHELL_SUCCESS);
}
