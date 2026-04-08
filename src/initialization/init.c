/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 16:24:26 by yolim             #+#    #+#             */
/*   Updated: 2026/04/08 17:00:25 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_minishell(t_minishell *minishell, char **envp)
{
	minishell->history_list = NULL;
	minishell->env_list = init_env(envp);
	minishell->tokens = NULL;
	minishell->ast = NULL;
	minishell->input = NULL;
	minishell->last_exit_status = 0;
	update_env("OLDPWD", NULL, minishell);
	increment_shlvl(minishell);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new = env_from_str(envp[i]);
		if (!new)
			break ;
		if (!head)
			head = new;
		else
			current->next = new;
		current = new;
		i++;
	}
	return (head);
}

void	increment_shlvl(t_minishell *minishell)
{
	char	*shlvl_str;
	int		shlvl_num;
	char	*new_shlvl_str;

	shlvl_str = get_var_value("SHLVL", minishell);
	if (shlvl_str[0] == '\0')
		update_env("SHLVL", "1", minishell);
	else
	{
		shlvl_num = ft_atoi(shlvl_str);
		shlvl_num++;
		new_shlvl_str = ft_itoa(shlvl_num);
		if (new_shlvl_str)
		{
			update_env("SHLVL", new_shlvl_str, minishell);
			free(new_shlvl_str);
		}
	}
	free(shlvl_str);
}
