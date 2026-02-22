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

// Helper to check valid bash identifier rules
static int is_valid_identifier(char *str)
{
    int i = 0;
    
    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int ft_unset(char **argv, t_env **env_head)
{
    int i = 1;
    int status = 0;

    while (argv[i])
    {
        if (!is_valid_identifier(argv[i]))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(argv[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            status = 1;
        }
        else
        {
            remove_env_node(env_head, argv[i]);
        }
        i++;
    }
    return (status);
}
