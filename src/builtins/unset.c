/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:12 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/06 16:44:40 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	ft_unset(char **argv, t_minishell *minishell)
{
	int	i;
	int	status;

	if (!argv || !argv[0])
		return (SHELL_SUCCESS);
	i = 1;
	status = SHELL_SUCCESS;
	while (argv[i])
	{
		if (!check_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = SHELL_FAILURE;
		}
		else
			remove_env_node(&minishell->env_list, argv[i]);
		i++;
	}
	return (status);
}

/*
Behaviour of unset in bash :
if the user provides 100 valid variables and 1 invalid one, all 100 valid
  ones are removed, but the shell still reports an exit status of 1 to let
  the user know something was wrong.
 */