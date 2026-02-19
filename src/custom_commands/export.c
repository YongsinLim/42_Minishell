/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:51 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/19 23:47:51 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0] || arg[0] == '=' 
		|| (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	process_export_arg(char *arg, t_env **env_head)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		key = ft_substr(arg, 0, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
		update_env(env_head, key, value);
		free(key);
		free(value);
	}
	else
	{
		key = ft_strdup(arg);
		update_env(env_head, key, NULL); // e.g. "export VAR" (no value)
		free(key);
	}
}

static int	print_export(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
	return (0);
}

int	ft_export(char **argv, t_env **env_head)
{
	int	i;
	int	status;

	if (!argv[1])
		return (print_export(*env_head));
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!check_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			process_export_arg(argv[i], env_head);
		i++;
	}
	return (status);
}