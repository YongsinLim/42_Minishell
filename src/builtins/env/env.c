/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 18:06:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/31 21:53:43 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_env(char **argv, t_minishell *minishell)
{
	t_env	*current;

	if (argv[1])
	{
		if (argv[1][0] == '-' && argv[1][1] != '\0')
		{
			ft_putstr_fd("minishell: env: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			ft_putstr_fd("env: usage: env\n", 2);
			return (2);
		}
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		return (127);
	}
	current = minishell->env_list;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=", 1);
			ft_putendl_fd(current->value, 1);
		}
		current = current->next;
	}
	return (SHELL_SUCCESS);
}

/*
declare local variable (t_env *current = minishell->env_list) bcos:
iterate through the list using a copy of the pointer, leaving the original
minishell->env_list safely pointing to the start of the list for future use
*/