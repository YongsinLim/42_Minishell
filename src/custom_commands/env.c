/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 18:06:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/11 18:06:59 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_env(t_env *env)
{
    while (env)
    {
        // Only print if value exists (export VAR vs export VAR=value)
        if (env->value) 
        {
            ft_putstr_fd(env->key, 1);
            ft_putstr_fd("=", 1);
            ft_putendl_fd(env->value, 1);
        }
        env = env->next;
    }
    return (0);
}