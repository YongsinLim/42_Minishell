/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 18:06:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/03 18:42:03 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_env(t_minishell *minishell)
{
    while (minishell->env_list)
    {
        // Only print if value exists (export VAR vs export VAR=value)
        if (minishell->env_list->value)
        {
            ft_putstr_fd(minishell->env_list->key, 1);
            ft_putstr_fd("=", 1);
            ft_putendl_fd(minishell->env_list->value, 1);
        }
        minishell->env_list = minishell->env_list->next;
    }
    return (SHELL_SUCCESS);
}