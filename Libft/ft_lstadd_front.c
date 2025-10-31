/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:20:44 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 14:20:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if ((lst == NULL) || (new == NULL))
		return ;
	new->next = *lst;
	*lst = new;
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
	t_list	*my_shopping_list_head = NULL;

	t_list *item_eggs = ft_lstnew("Eggs");
	t_list *item_bread = ft_lstnew("Bread");
	t_list *item_milk = ft_lstnew("Milk");

	if (!item_eggs || !item_bread || !item_milk)
	{
		printf("Failed to create list items.\n");
		free(item_eggs);
		free(item_bread);
		free(item_milk);
	}

	printf("Adding 'Bread' to the front...\n");
	ft_lstadd_front(&my_shopping_list_head, item_bread);
	print_list(my_shopping_list_head);

	printf("\nAdding 'Eggs' to the front...\n");
	ft_lstadd_front(&my_shopping_list_head, item_eggs);
	print_list(my_shopping_list_head);

	printf("\nAdding 'Milks' to the front...\n");
	ft_lstadd_front(&my_shopping_list_head, item_milk);
	print_list(my_shopping_list_head);
	return (0);
}
*/