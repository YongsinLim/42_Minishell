/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:21:54 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 15:09:15 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*current;

	count = 0;
	current = lst;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
int main(void)
{
    t_list *my_list = NULL;
    int size;

    // Test 1: Empty list
    printf("Test 1: Empty list\n");
    size = ft_lstsize(my_list);
    printf("Expected size: 0, Got: %d\n", size);
    printf("My List : %s\n", (char *) my_list);

    // Test 2: List with one element
    printf("Test 2: List with one element\n");
    t_list *node1 = ft_lstnew("Hello");
    ft_lstadd_front(&my_list, node1);

    size = ft_lstsize(my_list);
    printf("Expected size: 1, Got: %d\n", size);
	printf("My List : %s\n", (char *) my_list->content);

    // Test 3: List with multiple elements
    printf("Test 3: List with multiple elements\n");
    t_list *node2 = ft_lstnew("World");
    t_list *node3 = ft_lstnew("Sha");

    ft_lstadd_front(&my_list, node2); // my_list: node2 -> node1
    ft_lstadd_front(&my_list, node3); // my_list: node3 -> node2 -> node1

    size = ft_lstsize(my_list);
    printf("Expected size: 3, Got: %d\n", size);
	printf("My List : %s\n", (char *) my_list->content);
    return (0);
}
*/