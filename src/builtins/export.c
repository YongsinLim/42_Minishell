/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:51 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/30 12:16:56 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_key_cmp(t_env *a, t_env *b)
{
	size_t	len_a;
	size_t	len_b;
	size_t	n;

	len_a = ft_strlen(a->key);
	len_b = ft_strlen(b->key);
	n = len_a + len_b + 1;
	return (ft_strncmp(a->key, b->key, n));
}

void	swap_env_ptr(t_env **a, t_env **b)
{
	t_env	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_env_parts(t_env **arr, int count)
{
	int i;
	int j;

	i = 0;
	while (i < count - 1) {
		j = 0;
		while (j < count - i - 1)
		{
			if (env_key_cmp(arr[j], arr[j + 1]) > 0)
				swap_env_ptr(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

int	print_export(t_env *env)
{
	t_env	**arr;
	t_env	*current;
	int		count;
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
		if (arr[i]->value)
			printf("export %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		else
			printf("export %s\n", arr[i]->key);
		i++;;
	}
	free(arr);
	return (SHELL_SUCCESS);
}

int	export_invalid_option(char *arg)
{
	ft_putstr_fd("minishell: export: ", 2);
	if (arg[0] == '-' && arg[1] == '-')
		ft_putstr_fd("--", 2);
	else
	{
		ft_putstr_fd("-", 2);
		if (arg[1])
			ft_putchar_fd(arg[1], 2);
	}
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("export: usage: export [-nf] [name[=value] ...] or export -p\n", 2);
	return (2);
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
		if (arg[i] == '+' && arg[i + 1] == '=')
			return (TRUE);
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
	char	*clean_value;
	char	*old_value;
	char	*joined;
	int		append;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (update_env(arg, NULL, minishell));
	append = (equal_pos > arg && *(equal_pos - 1) == '+');
	key = ft_substr(arg, 0, (equal_pos - arg) - append);
	value = ft_strdup(equal_pos + 1);
	if (!key || !value)
		return (free(key), free(value));
	clean_value = ft_strtrim(value, "\r\n");
	if (!clean_value)
		return (free(key), free(value));
	if (append)
	{
		old_value = get_var_value(key, minishell);
		joined = ft_strjoin(old_value, clean_value);
		if (joined)
			update_env(key, joined, minishell);
		free(old_value);
		free(joined);
	}
	else
		update_env(key, clean_value, minishell);
	free(key);
	free(value);
	free(clean_value);
}

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
			return (export_invalid_option(argv[i]));
		i++;
	}
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
