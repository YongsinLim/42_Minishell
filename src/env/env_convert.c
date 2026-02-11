/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:51:40 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/11 17:51:40 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_env_nodes(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

static char	*join_env(char *key, char *value)
{
	char	*tmp;
	char	*res;

	if (!value)
		return (ft_strdup(key));
	tmp = ft_strjoin(key, "=");
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

char	**env_list_to_array(t_env *env)
{
	char	**array;
	int		count;
	int		i;

	count = count_env_nodes(env);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i] = join_env(env->key, env->value);
		env = env->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
