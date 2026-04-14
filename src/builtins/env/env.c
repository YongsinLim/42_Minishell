/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 18:06:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 19:50:51 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
if (argv[1]) - mean env with arguments -> execute by external command,
not builtin command

env only print env->key with env->value items.
*/
int	ft_env(char **argv, t_minishell *minishell)
{
	t_env	*current;

	if (argv[1])
		return (SHELL_FAILURE);
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