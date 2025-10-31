/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:38:13 by yolim             #+#    #+#             */
/*   Updated: 2025/06/03 19:04:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	if (del != NULL)
	{
		del(lst->content);
		free(lst);
	}
}

/*
void print_list(t_list *head)
{
	t_list *current = head;
	printf("List: ");
	while (current != NULL)
	{
		printf("\"%s\" -> ", (char *)current->content);
		current = current->next;
	}
	printf("NULL\n");
}

int	main(void)
{
	t_list	*head = NULL;
	t_list	*node_to_delete;
	char	*dynamic_content;
	t_list	*new_node;

	dynamic_content = ft_strdup("Hello Libft");
	new_node = ft_lstnew(dynamic_content);

	printf("Adding node with content: \"%s\"\n", (char *)new_node->content);
	ft_lstadd_front(&head, new_node);
	printf("List before deletion: ");
	print_list(head);

	printf("Deleting the head node...\n");
	node_to_delete = head;
	head = head->next;
	ft_lstdelone(node_to_delete, free);
	printf("List after deletion: ");
	print_list(head);
	return (0);
}
*/