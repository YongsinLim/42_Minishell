/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:49:03 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 17:54:45 by yolim            ###   ########.fr       */
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

char	*get_var_value(char *var_name, t_minishell *minishell)
{
	t_env	*current;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(minishell->last_exit_status));
	current = minishell->env_list;
	while (current)
	{
		if (ft_strncmp(current->key, var_name, ft_strlen(var_name) + 1) == 0)
		{
			if (current->value == NULL)
				return (ft_strdup(""));
			return (ft_strdup(current->value));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}

/*
identifier = the name of an environment variable
Bash identifier naming rules :
1st Character: Must be a letter (a-z, A-Z) or an underscore (_), cannot be a
number.
The Rest of the Characters: Can be letters, numbers, or underscores.
 */
int	check_valid_env_identifier(char *arg)
{
	int	i;

	if (!arg || !arg[0] || arg[0] == '='
		|| (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (FALSE);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
			return (TRUE);
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
