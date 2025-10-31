/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:35:08 by yolim             #+#    #+#             */
/*   Updated: 2025/09/21 19:10:31 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strmerge_free(char *str, char *buffer)
{
	char	*read_content;

	if (str == NULL)
		str = ft_strdup("");
	read_content = ft_strjoin(str, buffer);
	if (read_content == NULL)
	{
		free(buffer);
		return (NULL);
	}
	free(str);
	free(buffer);
	return (read_content);
}

char	*substr(char const *s, size_t len)
{
	size_t	i;
	char	*substr;

	if (len == 0)
		return (NULL);
	substr = malloc((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		*(substr + i) = *(s + i);
		i++;
	}
	*(substr + i) = '\0';
	return (substr);
}

void	free_gnl_list(t_node **head)
{
	t_node	*current;
	t_node	*next_node;

	current = *head;
	while (current)
	{
		next_node = current->next;
		free(current->read_content);
		free(current);
		current = next_node;
	}
	*head = NULL;
}
