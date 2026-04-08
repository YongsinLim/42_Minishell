/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:49:03 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/08 16:45:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*env_from_str(char *str)
{
	char	*equal_position;
	char	*key;
	char	*value;
	t_env	*new;

	equal_position = ft_strchr(str, '=');
	if (equal_position)
	{
		key = ft_substr(str, 0, equal_position - str);
		value = equal_position + 1;
	}
	else
	{
		key = ft_strdup(str);
		value = NULL;
	}
	new = new_env_node(key, value);
	free(key);
	return (new);
}

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
	if (last == NULL)
		minishell->env_list = new_env_node(key, value);
	else
		last->next = new_env_node(key, value);
}
