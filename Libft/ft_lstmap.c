/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:41:10 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 14:56:52 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	void	*temp_content;
	t_list	*mapped_tail;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	temp_content = f(lst->content);
	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
	{
		del(temp_content);
		return (NULL);
	}
	new_node->content = temp_content;
	mapped_tail = ft_lstmap(lst->next, f, del);
	new_node->next = mapped_tail;
	if (mapped_tail == NULL && lst->next != NULL)
	{
		del(new_node->content);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

/*
void print_string_list_simple(t_list *lst, const char *label) {
	printf("%s: ", label);
	if (!lst)
	{
		printf("NULL\n");
		return;
	}
	while (lst)
	{
		printf("\"%s\"", lst->content ? (char *)lst->content : "(null_content)");
		lst = lst->next;
		if (lst)
			printf(" -> ");
	}
	printf(" -> NULL\n");
}

void *duplicate_string_content(void *content)
{
	if (!content)
		return (NULL);
	return (ft_strdup((char *)content));
}

void free_string_content(void *content)
{
	free(content);
}

int	main(void)
{
	t_list *original_list = NULL;
	t_list *new_mapped_list = NULL;

	ft_lstadd_back(&original_list, ft_lstnew(ft_strdup("Hello")));
	ft_lstadd_back(&original_list, ft_lstnew(ft_strdup("World")));

	printf("Original list before ft_lstmap:\n");
	print_string_list_simple(original_list, "Original");

	new_mapped_list = ft_lstmap(original_list, duplicate_string_content,
			free_string_content);

	printf("\nLists after ft_lstmap:\n");
	print_string_list_simple(original_list, "Original (should be unchanged)");
	print_string_list_simple(new_mapped_list, "Mapped  ");

	// Clean up both lists
	ft_lstclear(&original_list, free_string_content);
	// Clears original list and its strdup'd content
	ft_lstclear(&new_mapped_list, free_string_content);
	// Clears new list and its strdup'd content
	return (0);
}
*/