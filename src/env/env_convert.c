/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:51:40 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/26 12:30:12 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	**env_list_to_array(t_env *env_list)
{
	char	**array;
	char	*temp;
	int		i;
	int		count;

	count = count_env_nodes(env_list);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		temp = ft_strjoin(env_list->key, "=");
		array[i] = ft_strjoin(temp, env_list->value);
		free(temp);
		i++;
		env_list = env_list->next;
	}
	array[i] = NULL;
	return (array);
}
