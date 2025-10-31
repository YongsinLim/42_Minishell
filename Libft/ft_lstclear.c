/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:19:29 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 11:58:06 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current_node;
	t_list	*next_node;

	if (lst == NULL || del == NULL)
		return ;
	current_node = *lst;
	while (current_node != NULL)
	{
		next_node = current_node->next;
		del(current_node->content);
		free(current_node);
		current_node = next_node;
	}
	*lst = NULL;
}

/*
int	main()
{
	t_list *head = NULL;
    t_list *node1, *node2;
    char   *content_str1;
    char   *content_str2;

    content_str1 = ft_strdup("First Item");
    content_str2 = ft_strdup("Second Item");
    node1 = ft_lstnew(content_str1);
    node2 = ft_lstnew(content_str2);

    // List will be: node2 ("Second Item") -> node1 ("First Item") -> NULL
    ft_lstadd_front(&head, node1);
    ft_lstadd_front(&head, node2);

    printf("List created. Initial head: %p\n", (void *)head);
    if (head)
    {
        printf("Content of first node: \"%s\"\n", (char *)head->content);
        if (head->next)
            printf("Content of 2nd node: \"%s\"\n", (char *)head->next->content);
    }

    printf("\nCalling ft_lstclear(&head, free)...\n");
    ft_lstclear(&head, free);

    if (head == NULL)
        printf("List successfully cleared. head is now NULL.\n");
    else
        printf("Error: List was not cleared. head is %p.\n", (void *)head);

    // 6. Test ft_lstclear on an already NULL list
    printf("\nCalling ft_lstclear on NULL list (head is %p)...\n", (void *)head);
    ft_lstclear(&head, free);
    if (head == NULL)
	{
        printf("Correctly handled ft_lstclear on NULL list. head remains NULL.");
		printf("\n");
	}
	else
        printf("Error: NULL list resulted in head = %p.", (void *)head);
    printf("\nTest finished.\n");
    return (0);
}
*/