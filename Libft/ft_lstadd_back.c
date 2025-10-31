/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:29:54 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 15:50:18 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_node;

	if ((lst == NULL) || (new == NULL))
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		last_node = *lst;
		while (last_node->next != NULL)
			last_node = last_node->next;
		last_node->next = new;
	}
}

/*
int	main(void)
{
	t_list	*my_shopping_list_head = NULL;

	t_list *item_bread = ft_lstnew("Bread");
	t_list *item_eggs = ft_lstnew("Eggs");
	t_list *item_milk = ft_lstnew("Milk");

	ft_lstadd_front(&my_shopping_list_head, item_bread);
	ft_lstadd_front(&my_shopping_list_head, item_eggs);
	ft_lstadd_front(&my_shopping_list_head, item_milk);

	t_list *item_choco = ft_lstnew("Choco");
	ft_lstadd_back(&my_shopping_list_head, item_choco);
	
	printf("%s\n", (char *)ft_lstlast(my_shopping_list_head)->content);
	return (0);
}
*/