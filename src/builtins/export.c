/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:51 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/04 15:40:45 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	return (SHELL_SUCCESS);
}

/*
identifier = the name of an environment variable
Bash identifier naming rules :
1st Character: Must be a letter (a-z, A-Z) or an underscore (_), cannot be a
number.
The Rest of the Characters: Can be letters, numbers, or underscores.
 */

int	check_valid_identifier(char *arg)
{
	int	i;

	if (!arg || !arg[0] || arg[0] == '='
		|| (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (FALSE);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	process_export_arg(char *arg, t_minishell *minishell)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		key = ft_substr(arg, 0, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
		update_env(key, value, minishell);
		free(key);
		free(value);
	}
	else
		update_env(arg, NULL, minishell); // e.g. "export VAR" (no value)
}

int	ft_export(char **argv, t_minishell *minishell)
{
	int	i;
	int	status;

	if (!argv[1])
		return (print_export(minishell->env_list));
	i = 1;
	status = SHELL_SUCCESS;
	while (argv[i])
	{
		if (!check_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = SHELL_FAILURE;
		}
		else
			process_export_arg(argv[i], minishell);
		i++;
	}
	return (status);
}