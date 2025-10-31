/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:34:47 by yolim             #+#    #+#             */
/*   Updated: 2025/09/21 19:46:21 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_node	*get_or_create_node(int fd, t_node **list_head)
{
	t_node	*current;
	t_node	*new_node;

	current = *list_head;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->read_content = NULL;
	new_node->next = *list_head;
	*list_head = new_node;
	return (new_node);
}

void	remove_fd(int fd, t_node **head)
{
	t_node	*current;
	t_node	*previous;

	current = *head;
	previous = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (previous == NULL)
				*head = current->next;
			else
				previous->next = current->next;
			free(current->read_content);
			free(current);
			break ;
		}
		previous = current;
		current = current->next;
	}
}

void	revise_read_content(char *str, unsigned int start)
{
	size_t	i;

	i = 0;
	while (*(str + start + i) != '\0')
	{
		*(str + i) = *(str + start + i);
		i++;
	}
	*(str + i) = '\0';
}

char	*get_result_line(t_node *current_node, ssize_t read_bytes,
		t_node **list_head)
{
	size_t	i;
	char	*result_line;
	char	*read_content;

	read_content = current_node->read_content;
	i = 0;
	while (*(read_content + i) != '\0')
	{
		if (*(read_content + i) == '\n')
		{
			result_line = substr(read_content, i + 1);
			revise_read_content(read_content, i + 1);
			return (result_line);
		}
		i++;
	}
	if (read_bytes == BUFFER_SIZE)
		return (get_next_line(current_node->fd));
	result_line = substr(read_content, i);
	revise_read_content(read_content, i);
	remove_fd(current_node->fd, list_head);
	return (result_line);
}

char	*get_next_line(int fd)
{
	static t_node	*list_head = NULL;
	t_node			*current_node;
	char			*buffer;
	ssize_t			read_bytes;

	if (fd == -1)
		return (free_gnl_list(&list_head), NULL);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	current_node = get_or_create_node(fd, &list_head);
	if (!current_node)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes == -1)
		return (free(buffer), NULL);
	buffer[read_bytes] = '\0';
	current_node->read_content = strmerge_free
		(current_node->read_content, buffer);
	if (current_node->read_content == NULL)
		return (NULL);
	return (get_result_line(current_node, read_bytes, &list_head));
}
