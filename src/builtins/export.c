/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:51 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 18:10:22 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_export(char **argv, t_minishell *minishell)
{
	int	i;
	int	status;

	if (!argv[1])
		return (print_export(minishell->env_list));
	i = 1;
	status = SHELL_SUCCESS;
	if (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		if (ft_strncmp(argv[i], "--", 3) != 0)
			return (report_invalid_export_option(argv[i]));
		i++;
	}
	while (argv[i])
	{
		if (!check_valid_env_identifier(argv[i]))
		{
			report_error("export: not a valid identifier", argv[i]);
			status = SHELL_FAILURE;
		}
		else
			process_export_arg(argv[i], minishell);
		i++;
	}
	return (status);
}

// if (ft_strncmp(arr[i]->key, "_", 2) != 0) to skip _=/usr/bin/env item
int	print_export(t_env *env)
{
	int		count;
	t_env	**arr;
	t_env	*current;
	int		i;

	count = count_env_nodes(env);
	arr = ft_calloc(count, sizeof(t_env *));
	if (!arr)
		return (SHELL_FAILURE);
	current = env;
	i = 0;
	while (current)
	{
		arr[i++] = current;
		current = current->next;
	}
	sort_env_parts(arr, count);
	i = 0;
	while (i < count)
	{
		print_export_node(arr[i]);
		i++;
	}
	free(arr);
	return (SHELL_SUCCESS);
}

void	print_export_node(t_env *node)
{
	if (ft_strncmp(node->key, "_", 2) == 0)
		return ;
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(node->key, 1);
	if (node->value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(node->value, 1);
		ft_putendl_fd("\"", 1);
	}
	else
		ft_putchar_fd('\n', 1);
}

// += mean append new value into existing env key's value
void	process_export_arg(char *arg, t_minishell *minishell)
{
	char	*equal_pos;
	int		append;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (update_env(arg, NULL, minishell));
	if (equal_pos > arg && *(equal_pos - 1) == '+')
		append = TRUE;
	else
		append = FALSE;
	key = ft_substr(arg, 0, (equal_pos - arg) - append);
	value = ft_strdup(equal_pos + 1);
	if (!key || !value)
		return (free(key), free(value));
	set_export_value(key, value, append, minishell);
	free(key);
	free(value);
}

void	set_export_value(char *key, char *clean_value, int append,
	t_minishell *minishell)
{
	char	*old_value;
	char	*joined;

	if (!append)
		return (update_env(key, clean_value, minishell));
	old_value = get_var_value(key, minishell);
	joined = ft_strjoin(old_value, clean_value);
	if (joined)
		update_env(key, joined, minishell);
	free(old_value);
	free(joined);
}
