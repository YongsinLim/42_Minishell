/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:47:12 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/19 23:47:12 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unset(char **argv, t_env **env_head)
{
    int i;

    i = 1;
    while (argv[i])
    {
        remove_env_node(env_head, argv[i]);
        i++;
    }
    return (0);
}