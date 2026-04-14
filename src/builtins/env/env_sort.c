/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:49:51 by yolim             #+#    #+#             */
/*   Updated: 2026/04/14 16:52:03 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	count_env_nodes(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

void	sort_env_parts(t_env **arr, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (env_key_cmp(arr[j], arr[j + 1]) > 0)
				swap_env_ptr(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

int	env_key_cmp(t_env *a, t_env *b)
{
	size_t	len_a;
	size_t	len_b;
	size_t	n;

	len_a = ft_strlen(a->key);
	len_b = ft_strlen(b->key);
	if (len_a > len_b)
		n = len_a + 1;
	else
		n = len_b + 1;
	return (ft_strncmp(a->key, b->key, n));
}

void	swap_env_ptr(t_env **a, t_env **b)
{
	t_env	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
