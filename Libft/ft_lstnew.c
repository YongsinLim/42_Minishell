/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:08:25 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:31:24 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

/*
int	main(void)
{
	int my_data = 42;
	
	t_list *new_node = ft_lstnew(&my_data);
	printf("Result : %d\n", (*(int *)new_node->content));

	char	*str = "123";

	new_node = ft_lstnew(str);
	printf("Result : %s\n", (char *)new_node->content);

	return (0);
}
*/