/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 18:06:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/03 18:58:24 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_env(t_minishell *minishell)
{
    t_env *current;

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