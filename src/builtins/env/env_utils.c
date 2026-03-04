/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:49:03 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/04 16:57:24 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = ft_calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new;
	int		i;
	char	*equal_position;
	char	*key;
	char	*value;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp && envp[i])
	{
		equal_position = ft_strchr(envp[i], '=');
		if (equal_position)
		{
			key = ft_substr(envp[i], 0, equal_position - envp[i]);
			value = equal_position + 1;
		}
		else
		{
			key = ft_strdup(envp[i]);
			value = NULL;
		}
		new = new_env_node(key, value);
		free(key);
		if (!head)
		{
			head = new;
			current = new;
		}
		else
			current->next = new;
		current = new;
		i++;
	}
	return (head);
}

void	update_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*current;
	t_env	*last;

	current = minishell->env_list;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (!value)
				return ;
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		last = current;
		current = current->next;
	}
	// Key Not found - create new node
	if (last == NULL)
		minishell->env_list = new_env_node(key, value);
	else
		last->next = new_env_node(key, value);;
}
