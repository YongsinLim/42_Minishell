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

void	add_to_history(char *command, t_history **history_list)
{
	t_history	*new_node;
	t_history	*last;

	if (!command || command[0] == '\0')
		return ;
	new_node = malloc(sizeof(t_history));
	if (!new_node)
		return ;
	new_node->command = ft_strdup(command);
	if (!new_node->command)
	{
		free(new_node);
		return ;
	}
	new_node->next = NULL;
	if (*history_list == NULL)
		*history_list = new_node;
	else
	{
		last = *history_list;
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
