/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:12 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 19:20:21 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Behaviour of unset in bash :
if the user provides 100 valid variables and 1 invalid one, all 100 valid
ones are removed, but the shell still reports an exit status of 1 to let
the user know something was wrong.
 */
int	ft_unset(char **argv, t_minishell *minishell)
{
	int	i;

	if (!argv || !argv[0])
		return (SHELL_SUCCESS);
	i = 1;
	if (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		if (ft_strncmp(argv[i], "--", 3) != 0)
		{
			report_unset_error(argv, i, "invalid option");
			return (SYNTAX_ERROR);
		}
		i++;
	}
	while (argv[i])
	{
		if (!check_valid_unset_identifier(argv[i]))
		{
			report_unset_error(argv, i, "not a valid identifier");
			return (SHELL_FAILURE);
		}
		remove_env_node(&minishell->env_list, argv[i]);
		i++;
	}
	return (SHELL_SUCCESS);
}

void	report_unset_error(char **argv, int i, char *msg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(argv[i], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

// first char cant use -, as it might treat as option (e.g. -t -v)
int	check_valid_unset_identifier(char *arg)
{
	if (!arg)
		return (FALSE);
	if (arg[0] == '-')
		return (FALSE);
	return (TRUE);
}

void	remove_env_node(t_env **env_head, char *key)
{
	t_env	*current;
	t_env	*previous;

	current = *env_head;
	previous = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (previous == NULL)
				*env_head = current->next;
			else
				previous->next = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
