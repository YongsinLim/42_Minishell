/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_delete.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:46:28 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/19 23:46:28 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void remove_env_node(t_env **env_head, char *key)
{
    t_env *curr;
    t_env *prev;

    curr = *env_head;
    prev = NULL;
    while (curr)
    {
        if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
        {
            if (prev == NULL)
                *env_head = curr->next;
            else
                prev->next = curr->next;
            
            free(curr->key);
            if (curr->value)
                free(curr->value);
            free(curr);
            return ;
        }
        prev = curr;
        curr = curr->next;
    }
}